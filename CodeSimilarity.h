#ifndef __CODE_SIMILARITY_H
#define __CODE_SIMILARITY_H

#include "HashTableFast.h"
#include <stdint.h>

#define MAX_PROGRAM_ID_LEN 18
#define MAX_PROGRAM_SEG_COUNT 65535
#define PROGRAM_HASH_TABLE_MOD 101
#define SIMILARITY_THRESHOLD 0.95

struct ProgramList {
    struct HashTable *processedProgramTable;
    char **programList;
    size_t *programKeyStreamLen;
    size_t *programFunctionCount;
    double *programMainRate;
    double *firstSecondRate;
    size_t programCount;
};

struct ProgramList generateProgramList(char *src, uint64_t *identifierHashList);
double getSimilarity(char *keyStreamA, char *keyStreamB, size_t streamALen,
                     size_t streamBLen);
void destroyProgramList(struct ProgramList *target);
int editdistDP(char *str1, char *str2);

#endif