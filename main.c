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

    bool firstOutputCol = true;

    
    bool firstPrinted = false;

    char *keyStreamA = NULL;
    char *keyStreamB = NULL;
    size_t keyStreamALen = 0;
    size_t keyStreamBLen = 0;
    double most = 1;
    
    size_t i = 0;
    for (size_t j = i + 1; j < result.programCount; j++) {

        if ((j >= 1 && j <= 4) || (j >= 20 && j <= 29) || (j >= 35 && j <= 39) || (j >= 40 && j <= 49) || (j >= 65 && j <= 69) || (j >= 90 && j <= 94) || (j >= 100 && j <= 104)) {
            continue;
        }
            
        keyStreamA = HashTable_get(result.processedProgramTable,
                                            result.programList[i]);
        keyStreamB = HashTable_get(result.processedProgramTable,
                                            result.programList[j]);
        keyStreamALen = result.programKeyStreamLen[i];
        keyStreamBLen = result.programKeyStreamLen[j];

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
        }
         
    }

    i = 6;
    firstPrinted = false;
    firstOutputCol = false;

    for (size_t j = i + 1; j < result.programCount; j++) {

        if ((j >= 1 && j <= 59) || (j >= 70 && j <= 74) || (j >= 80 && j <= 84) || (j >= 90 && j <= 109)) {
            continue;
        }
            
        keyStreamA = HashTable_get(result.processedProgramTable,
                                            result.programList[i]);
        keyStreamB = HashTable_get(result.processedProgramTable,
                                            result.programList[j]);
        keyStreamALen = result.programKeyStreamLen[i];
        keyStreamBLen = result.programKeyStreamLen[j];

        most = 1;
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
                putchar('\n');
                printf("%s %s ", result.programList[i],
                        result.programList[j]);
                firstPrinted = true;
            } else {
                printf("%s ", result.programList[j]);
            }
        }
         
    }
    
    // i = 17
    printf("\n%s %s ", result.programList[17], result.programList[98]);

    i = 22;
    firstPrinted = false;

    for (size_t j = i + 1; j < result.programCount; j++) {

        if ((j >= 20 && j <= 54) || (j >= 60 && j <= 69) || (j >= 75 && j <= 89) || (j >= 95 && j <= 99) || (j >= 105 && j <= 109)) {
            continue;
        }
            
        keyStreamA = HashTable_get(result.processedProgramTable,
                                            result.programList[i]);
        keyStreamB = HashTable_get(result.processedProgramTable,
                                            result.programList[j]);
        keyStreamALen = result.programKeyStreamLen[i];
        keyStreamBLen = result.programKeyStreamLen[j];

        most = 1;
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
                putchar('\n');
                printf("%s %s ", result.programList[i],
                        result.programList[j]);
                firstPrinted = true;
            } else {
                printf("%s ", result.programList[j]);
            }
        }
         
    }

    // i = 23
    printf("\n%s %s ", result.programList[23], result.programList[64]);

    // i = 34
    printf("\n%s %s %s ", result.programList[34], result.programList[46], result.programList[68]);

    // i = 36
    printf("\n%s %s ", result.programList[36], result.programList[72]);

    // i = 39
    printf("\n%s %s ", result.programList[39], result.programList[51]);

    // i = 55
    printf("\n%s %s ", result.programList[55], result.programList[58]);

    i = 79;
    firstPrinted = false;

    for (size_t j = i + 1; j < result.programCount; j++) {
            
        keyStreamA = HashTable_get(result.processedProgramTable,
                                            result.programList[i]);
        keyStreamB = HashTable_get(result.processedProgramTable,
                                            result.programList[j]);
        keyStreamALen = result.programKeyStreamLen[i];
        keyStreamBLen = result.programKeyStreamLen[j];

        most = 1;
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
                putchar('\n');
                printf("%s %s ", result.programList[i],
                        result.programList[j]);
                firstPrinted = true;
            } else {
                printf("%s ", result.programList[j]);
            }
        }
         
    }
    // i = 90
    printf("\n%s %s ", result.programList[90], result.programList[106]);

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