#ifndef __KEY_INFO_STREAM_H
#define __KEY_INFO_STREAM_H

#include "HashTableFast.h"
#include "IdentifierHash.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_FUNCTION_NAME_LEN 255
#define MAX_IDENTIFIER_LEN 255
#define USER_FUNCTION_IDENTIFIER "FUNC"
#define FUNCTION_HASH_TABLE_MOD 233
#define MAX_USER_FUNCTION_COUNT 255

struct ProgramKeyInfo {
    struct HashTable *functionTable;
    char **userFunctionList;
    size_t userFunctionCount; // called in main, order by calling, functions in
                              // the list MAY NOT defined in the program
    char *programKeyInfoStream;
};

void generateFunctionKeyInfoStream(char *source, char *keyInfoStream,
                                   uint64_t *identifierHashList,
                                   char **userFunctionList,
                                   bool saveUserFunction,
                                   char **userDefinedFunctionList);
// source could end with '\f' or '\0'
struct ProgramKeyInfo generateProgramKeyInfo(char *source,
                                             uint64_t *identifierHashList);

size_t generateProgramKeyInfoStreamStr(struct ProgramKeyInfo *programKeyInfo,
                                       char **ptrToResult);

void destroyProgramKeyInfo(struct ProgramKeyInfo *target);

#endif