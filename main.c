#include "CodeSimilarity.h"
#include "HashTableFast.h"
#include "IdentifierHash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CODES_LEN 1024 * 1024 * 64

size_t max(size_t a, size_t b);

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

    bool *alreadyCheckedList =
        (bool *)calloc(result.programCount, sizeof(bool));

    bool firstOutputCol = true;
    for (size_t i = 0; i < result.programCount; i++) {
        bool alreadyChecked = alreadyCheckedList[i];

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
                    alreadyCheckedList[i] = true;
                    alreadyCheckedList[j] = true;
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