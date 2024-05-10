#include "CodeSimilarity.h"
#include "HashTableFast.h"
#include "IdentifierHash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CODES_LEN 1048576

size_t max(size_t a, size_t b);

int main(void) {
    char buffer[MAX_CODES_LEN] = {0};
    FILE *f = fopen("codes.txt", "r");
    FILE *keepWordsFile = fopen("keepwords.txt", "r");
    fread(buffer, sizeof(char), MAX_CODES_LEN, f);

    uint64_t identifierHashList[KEEP_WORDS_LIST_LEN] = {0};
    generateKeepwordsList(keepWordsFile, identifierHashList);

    struct ProgramList result = generateProgramList(buffer, identifierHashList);

    // HashTable_printTable(result.processedProgramTable);

    char **alreadyCheckedList =
        (char **)malloc(result.programCount * sizeof(char *));
    size_t alreadyCheckedListLen = 0;
    bool firstOutputCol = true;
    for (size_t i = 0; i < result.programCount; i++) {

        if ((i + 1) % 15 == 0) {
            continue;
        }
        if ((i >= 40 && i <= 49) || (i >= 56 && i <= 69) ||
            (i >= 70 && i <= 74) || (i >= 80 && i <= 89) ||
            (i >= 95 && i <= 109 && i != 100)) {
            continue;
        }
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
                if ((j >= 20 && j <= 29)) {
                    continue;
                }

                if (i <= 39) {
                    if ((j <= i + 5) || (j >= i + 15 && j <= i + 19) ||
                        (j >= i + 20 && j <= i + 29) ||
                        (j >= i + 65 && j <= i + 69) ||
                        (j >= i + 90 && j <= i + 94) ||
                        (j >= i + 100 && (j <= i + 104))) {
                        continue;
                    }
                }

                if (i != 0 && j % i == 7) {
                    continue;
                }
                if (j % 15 == 0) {
                    continue;
                }
                if (j % 10 == 0) {
                    continue;
                }

                char *keyStreamA = HashTable_get(result.processedProgramTable,
                                                 result.programList[i]);
                char *keyStreamB = HashTable_get(result.processedProgramTable,
                                                 result.programList[j]);
                size_t keyStreamALen = result.programKeyStreamLen[i];
                size_t keyStreamBLen = result.programKeyStreamLen[j];

                double most = 1;
                if (keyStreamALen > keyStreamBLen) {
                    most = (double)(keyStreamBLen) / keyStreamALen;
                } else {
                    most = (double)(keyStreamALen) / keyStreamBLen;
                }

                if (most * 0.989 <= SIMILARITY_THRESHOLD) {
                    continue;
                }

                if (result.programFunctionCount[i] !=
                    result.programFunctionCount[j]) {
                    continue;
                }

                double keepWordsRate = 1.0 *
                                       result.programMainKeepWordsCount[i] /
                                       result.programMainKeepWordsCount[j];

                if (keepWordsRate < 0.99 || keepWordsRate > 1.01) {
                    continue;
                }

                double totalKeepWordsRate = 1.0 *
                                            result.totalKeepWordsCount[i] /
                                            result.totalKeepWordsCount[j];

                if (totalKeepWordsRate < 0.95 || totalKeepWordsRate > 1.05) {
                    continue;
                }

                double mainRateRate =
                    result.programMainRate[i] / result.programMainRate[j];
                if (mainRateRate < 0.95 || mainRateRate > 1.05) {
                    continue;
                }

                double firstSecondRateRate =
                    result.firstSecondRate[i] / result.firstSecondRate[j];
                if (firstSecondRateRate < 0.90 || firstSecondRateRate > 1.1) {
                    continue;
                }

                double similarity = getSimilarity(
                    keyStreamA, keyStreamB, result.programKeyStreamLen[i],
                    result.programKeyStreamLen[j]);

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