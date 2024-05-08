#include "CodeSimilarity.h"
#include "HashTableFast.h"
#include "KeyInfoStream.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static size_t max(size_t a, size_t b);

struct ProgramList generateProgramList(char *src,
                                       uint64_t *identifierHashList) {

    struct ProgramList result = {NULL, NULL, NULL, NULL, NULL, 0};

    result.processedProgramTable =
        HashTable_createTable(PROGRAM_HASH_TABLE_MOD);
    result.programList =
        (char **)malloc(MAX_PROGRAM_SEG_COUNT * sizeof(char *));
    result.programKeyStreamLen =
        (size_t *)malloc(MAX_PROGRAM_SEG_COUNT * sizeof(size_t));
    result.programFunctionCount =
        (size_t *)malloc(MAX_PROGRAM_SEG_COUNT * sizeof(size_t));
    result.programMainRate =
        (double *)malloc(MAX_PROGRAM_SEG_COUNT * sizeof(double));
    while (1) {
        // skip space
        while (*src == ' ' || *src == '\n' || *src == '\r' || *src == '\t' ||
               *src == '\r') {
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
               src - currentProgramSectionBegin);
        currentProgramId[src - currentProgramSectionBegin] = '\0';

        // skip space

        while (*src == ' ' || *src == '\n' || *src == '\r' || *src == '\t' ||
               *src == '\r') {
            src++;
        }

        char *currentProgramBegin = src;

        struct ProgramKeyInfo programKeyInfo =
            generateProgramKeyInfo(currentProgramBegin, identifierHashList);

        char *programKeyInfoStream =
            (char *)malloc((programKeyInfo.streamLen + 1) * sizeof(char));
        generateProgramKeyInfoStreamStr(&programKeyInfo, programKeyInfoStream);

        HashTable_insert(result.processedProgramTable, currentProgramId,
                         programKeyInfoStream);
        result.programList[result.programCount] = currentProgramId;
        result.programKeyStreamLen[result.programCount] =
            programKeyInfo.streamLen;
        result.programFunctionCount[result.programCount] =
            programKeyInfo.userFunctionCount;

        char *mainBody = HashTable_get(programKeyInfo.functionTable, "main");
        result.programMainRate[result.programCount] =
            1.0 * strlen(mainBody) / programKeyInfo.streamLen;

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
                            size_t streamALen, size_t streamBLen) {
    return 1 - (double)(editdistDP(keyStreamA, keyStreamB)) /
                   max(streamALen, streamBLen);
}

void destroyProgramList(struct ProgramList *target) {
    HashTable_destroyTable(target->processedProgramTable);
    free(target->programFunctionCount);
    free(target->programKeyStreamLen);
    free(target->programList);
    free(target->programMainRate);
}

size_t max(size_t a, size_t b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}