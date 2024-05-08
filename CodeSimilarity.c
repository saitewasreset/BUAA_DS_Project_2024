#include "CodeSimilarity.h"
#include "HashTableFast.h"
#include "KeyInfoStream.h"
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
                            size_t maxStreamLen) {
    return 1 - (double)(editdistDP(keyStreamA, keyStreamB)) / maxStreamLen;
}

void destroyProgramList(struct ProgramList *target) {
    HashTable_destroyTable(target->processedProgramTable);
    free(target->programKeyStreamLen);
    free(target->programList);
}