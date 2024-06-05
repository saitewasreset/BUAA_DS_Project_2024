#include "CodeSimilarity.h"
#include "HashTableFast.h"
#include "IdentifierHash.h"
#include "opt.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODES_LEN 1024 * 1024 * 64

size_t max(size_t a, size_t b);
void generateCharacterCount(char *str, unsigned int *target);
void dumpInfo(struct ProgramList *result);

char *buffer = NULL;

int main(void) {
    buffer = (char *)malloc(MAX_CODES_LEN * sizeof(char));
    FILE *f = fopen("codes.txt", "r");
    FILE *keepWordsFile = fopen("keepwords.txt", "r");
    fread(buffer, sizeof(char), MAX_CODES_LEN, f);

    uint64_t identifierHashList[KEEP_WORDS_LIST_LEN] = {0};
    generateKeepwordsList(keepWordsFile, identifierHashList);

    struct ProgramList result = generateProgramList(buffer, identifierHashList);

    bool *alreadyCheckedList =
        (bool *)calloc(result.programCount, sizeof(bool));

    unsigned int(*characterCountList)[128] =
        calloc(result.programCount, 128 * sizeof(unsigned int));
    bool *characterCountGenerated =
        (bool *)calloc(result.programCount, sizeof(bool));

    bool firstOutputCol = true;
    size_t totalCmp = 0;
    size_t skipByLen = 0;
    size_t skipBycD = 0;
    size_t skipByF = 0;
    // dumpInfo(&result);
    for (size_t i = 0; i < result.programCount; i++) {
        bool alreadyChecked = alreadyCheckedList[i];

        bool firstPrinted = false;
        if (!alreadyChecked) {
            for (size_t j = i + 1; j < result.programCount; j++) {
                totalCmp++;
                char *keyStreamA = HashTable_get(result.processedProgramTable,
                                                 result.programList[i]);
                char *keyStreamB = HashTable_get(result.processedProgramTable,
                                                 result.programList[j]);

                size_t keyStreamALen = result.programKeyStreamLen[i];
                size_t keyStreamBLen = result.programKeyStreamLen[j];

                size_t maxStreamLen = max(keyStreamALen, keyStreamBLen);

                size_t maxDist = maxStreamLen * (1 - SIMILARITY_THRESHOLD) + 1;

                double similarityThreshold =
                    1.0 * maxStreamLen * (1 - SIMILARITY_THRESHOLD);

                double most = 1;
                if (keyStreamALen > keyStreamBLen) {
                    most = (double)(keyStreamBLen) / (double)keyStreamALen;
                } else {
                    most = (double)(keyStreamALen) / (double)keyStreamBLen;
                }

                if (most <= SIMILARITY_THRESHOLD) {
                    skipByLen++;
                    continue;
                }

                if (!characterCountGenerated[i]) {
                    generateCharacterCount(keyStreamA, characterCountList[i]);
                    characterCountGenerated[i] = true;
                }

                if (!characterCountGenerated[j]) {
                    generateCharacterCount(keyStreamB, characterCountList[j]);
                    characterCountGenerated[j] = true;
                }

                unsigned int cDistance = 0;
                for (int k = 0; k < 128; k++) {
                    if (characterCountList[i][k] > characterCountList[j][k]) {
                        cDistance +=
                            characterCountList[i][k] - characterCountList[j][k];
                    } else {
                        cDistance +=
                            characterCountList[j][k] - characterCountList[i][k];
                    }
                }

                if (cDistance >= (maxStreamLen / 10 - 1)) {
                    skipBycD++;
                    continue;
                }

                int hDist = hammingDist(keyStreamA, keyStreamB,
                                        (int)keyStreamALen, (int)keyStreamBLen);

                if (hDist < (int)maxDist - 1) {
                    goto SIM;
                }
                /*
                int ed = rockEditDistance(keyStreamA, keyStreamB, 600);
                int edStd = editdistDP(keyStreamA, keyStreamB);

                if (edStd < 600) {
                    if (edStd == ed) {
                        printf("!%zu:%zu: %d\n", i, j, edStd);
                    } else {
                        printf("Failed!\n");
                        return 0;
                    }
                } else {
                    printf(".%zu:%zu: %d\n", i, j, edStd);
                }
                */
                int ed = rockEditDistance(
                    keyStreamA, keyStreamB, result.programKeyStreamLen[i],
                    result.programKeyStreamLen[j], maxDist);

                if (ed <= similarityThreshold) {
                SIM:
                    if (!firstPrinted) {
                        if (!firstOutputCol) {
                            putchar('\n');
                        } else {
                            firstOutputCol = false;
                        }
                        printf("%s %s ", result.programList[i],
                               result.programList[j]);
                        firstPrinted = true;
                    } else {
                        printf("%s ", result.programList[j]);
                    }
                    alreadyCheckedList[i] = true;
                    alreadyCheckedList[j] = true;
                }
            }
        }
    }
    // printf("\n%zu %zu %zu %zu\n", totalCmp, skipByLen, skipBycD, skipByF);
    destroyBeginMap();
    free(characterCountList);
    free(characterCountGenerated);
    free(alreadyCheckedList);
    destroyProgramList(&result);
    free(buffer);
    fclose(keepWordsFile);
    fclose(f);

    return 0;
}

void generateCharacterCount(char *str, unsigned int *target) {
    while (*str != '\0') {
        target[(uint8_t)(*str)]++;
        str++;
    }
}

inline size_t max(size_t a, size_t b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}
void dumpInfo(struct ProgramList *result) {
    struct HashTable *table = result->processedProgramTable;
    for (size_t i = 0; i < result->programCount; i++) {
        char *currentProgramId = result->programList[i];
        char *currentStream = HashTable_get(table, currentProgramId);

        printf("%zu:%s: Len = %zu\n%s\n\n", i, currentProgramId,
               strlen(currentStream), currentStream);
    }

    puts("-------------------------------------------------------------");

    for (size_t i = 0; i < result->programCount; i++) {
        for (size_t j = i + 1; j < result->programCount; j++) {
            char *streamA = HashTable_get(table, result->programList[i]);
            char *streamB = HashTable_get(table, result->programList[j]);
            int ed = editdistDP(streamA, streamB);

            printf("(%zu:%zu):(%s:%s): ed = %d\n", i, j, result->programList[i],
                   result->programList[j], ed);
        }
    }
}