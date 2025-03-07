#include "CodeSimilarity.h"
#include "HashTableFast.h"
#include "KeyInfoStream.h"
#include "opt.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct ProgramList generateProgramList(char *src,
                                       uint64_t *identifierHashList) {

    struct ProgramList result = {NULL, NULL, NULL, 0};

    result.processedProgramTable =
        HashTable_createTable(PROGRAM_HASH_TABLE_MOD);
    result.programList =
        (char **)malloc(MAX_PROGRAM_SEG_COUNT * sizeof(char *));
    result.programKeyStreamLen =
        (size_t *)malloc(MAX_PROGRAM_SEG_COUNT * sizeof(size_t));

    while (1) {
        // skip space
        while (isspace(*src)) {
            src++;
        }
        char *currentProgramSectionBegin = src;
        // Program id
        while (isdigit(*src)) {
            src++;
        }
        // will be freed in destroyHashTable
        char *currentProgramId = (char *)malloc(
            (size_t)(src - currentProgramSectionBegin + 1) * sizeof(char));
        memcpy(currentProgramId, currentProgramSectionBegin,
               (size_t)(src - currentProgramSectionBegin));
        currentProgramId[src - currentProgramSectionBegin] = '\0';

        // skip space

        while (isspace(*src)) {
            src++;
        }

        char *currentProgramBegin = src;

        struct ProgramKeyInfo programKeyInfo =
            generateProgramKeyInfo(currentProgramBegin, identifierHashList);

        char *programKeyInfoStream = NULL;
        size_t programKeyInfoStreamLen = generateProgramKeyInfoStreamStr(
            &programKeyInfo, &programKeyInfoStream);

        HashTable_insert(result.processedProgramTable, currentProgramId,
                         programKeyInfoStream);
        result.programList[result.programCount] = currentProgramId;
        result.programKeyStreamLen[result.programCount] =
            programKeyInfoStreamLen;
        result.programCount++;
        // program
        while (*src != '\f' && *src != '\0') {
            src++;
        }
        destroyProgramKeyInfo(&programKeyInfo);
        // input ends
        if (*src == '\0') {
            break;
        } else {
            // skip '\f'
            src++;
            currentProgramSectionBegin = src;
            if (*src == '\0') {
                break;
            }
        }
    }

    return result;
}

inline double getSimilarity(char *keyStreamA, char *keyStreamB,
                            size_t keyStreamALen, size_t keyStreamBLen,
                            size_t maxDist) {
    size_t maxStreamLen =
        keyStreamALen > keyStreamBLen ? keyStreamALen : keyStreamBLen;
    return 1 -
           (double)(editdistDP(keyStreamA, keyStreamB)) / (double)maxStreamLen;
}

int hammingDist(char *str1, char *str2, int str1Len, int str2Len) {
    int delta = str1Len - str2Len;
    int minLen = str1Len;
    if (delta < 0) {
        delta = -delta;
    } else {
        minLen = str2Len;
    }
    int result = 0;
    for (int i = 0; i < minLen; i++) {
        if (str1[i] != str2[i]) {
            result++;
        }
    }

    result += delta;
    return result;
}

void destroyProgramList(struct ProgramList *target) {
    HashTable_destroyTable(target->processedProgramTable);
    free(target->programKeyStreamLen);
    free(target->programList);
}