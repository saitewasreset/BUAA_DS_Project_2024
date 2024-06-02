#include "CodeSimilarity.h"
#include "HashTableFast.h"
#include "IdentifierHash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODES_LEN 1024 * 1024 * 64

size_t max(size_t a, size_t b);
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

    // HashTable_printTable(result.processedProgramTable);

    dumpInfo(&result);

    char **alreadyCheckedList =
        (char **)malloc(result.programCount * sizeof(char *));
    size_t alreadyCheckedListLen = 0;
    bool firstOutputCol = true;
    for (size_t i = 0; i < result.programCount; i++) {
        bool alreadyChecked = false;
        for (size_t j = 0; j < alreadyCheckedListLen; j++) {
            if (alreadyCheckedList[j] == result.programList[i]) {
                alreadyChecked = true;
                break;
            }
        }

        bool firstPrinted = false;
        if (!alreadyChecked) {
            for (size_t j = i + 1; j < result.programCount; j++) {
                char *keyStreamA = HashTable_get(result.processedProgramTable,
                                                 result.programList[i]);
                char *keyStreamB = HashTable_get(result.processedProgramTable,
                                                 result.programList[j]);
                double similarity =
                    getSimilarity(keyStreamA, keyStreamB,
                                  max(result.programKeyStreamLen[i],
                                      result.programKeyStreamLen[j]));

                if (similarity > SIMILARITY_THRESHOLD) {
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
                    alreadyCheckedList[alreadyCheckedListLen] =
                        result.programList[i];
                    alreadyCheckedListLen++;
                    alreadyCheckedList[alreadyCheckedListLen] =
                        result.programList[j];
                    alreadyCheckedListLen++;
                }
            }
        }
    }
    free(alreadyCheckedList);
    destroyProgramList(&result);
    free(buffer);
    fclose(keepWordsFile);
    fclose(f);

    return 0;
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

        printf("%zu:%s: Len = %zu realLen = %zu\n%s\n\n", i, currentProgramId,
               result->programKeyStreamLen[i], strlen(currentStream),
               currentStream);
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