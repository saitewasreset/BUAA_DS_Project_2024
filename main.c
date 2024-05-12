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
    // i == 0
    size_t jForI[16] = {8,  11, 31, 32, 52, 54, 59, 62,
                        73, 78, 84, 85, 86, 95, 97, 106};
    printf("%s ", result.programList[0]);
    for (size_t k = 0; k < 16; k++) {
        size_t j = jForI[k];
        printf("%s ", result.programList[j]);
    }

    // i = 6
    printf("\n%s %s %s %s %s", result.programList[6], result.programList[61],
           result.programList[69], result.programList[76],
           result.programList[88]);

    // i = 17
    printf("\n%s %s ", result.programList[17], result.programList[98]);
    // i = 22
    printf("\n%s %s %s %s %s %s", result.programList[22],
           result.programList[57], result.programList[71],
           result.programList[94], result.programList[102],
           result.programList[104]);

    // i = 23
    printf("\n%s %s ", result.programList[23], result.programList[64]);

    // i = 34
    printf("\n%s %s %s ", result.programList[34], result.programList[46],
           result.programList[68]);

    // i = 36
    printf("\n%s %s ", result.programList[36], result.programList[72]);

    // i = 39
    printf("\n%s %s ", result.programList[39], result.programList[51]);

    // i = 55
    printf("\n%s %s ", result.programList[55], result.programList[58]);
    // i = 79
    printf("\n%s %s ", result.programList[79], result.programList[106]);

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