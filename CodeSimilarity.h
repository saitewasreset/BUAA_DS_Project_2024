#ifndef __CODE_SIMILARITY_H
#define __CODE_SIMILARITY_H

#include "HashTableFast.h"
#include <stdint.h>

#define MAX_PROGRAM_ID_LEN 64
#define MAX_PROGRAM_SEG_COUNT 255
#define PROGRAM_HASH_TABLE_MOD 233
#define SIMILARITY_THRESHOLD 0.95

struct ProgramList {
    struct HashTable *processedProgramTable;
    char **programList;
    size_t *programKeyStreamLen;
    size_t programCount;
};

struct ProgramList generateProgramList(char *src, uint64_t *identifierHashList);
double getSimilarity(char *keyStreamA, char *keyStreamB, size_t keyStreamALen,
                     size_t keyStreamBLen, size_t maxDist);
void destroyProgramList(struct ProgramList *target);
int hammingDist(char *str1, char *str2, int str1Len, int str2Len);

int editdistDP(char *str1, char *str2);
int rockEditDistance(char *s1, char *s2, int strLen1, int strLen2, int maxDist);
#endif