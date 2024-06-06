#include "KeyInfoStream.h"
#include "HashTableFast.h"
#include "IdentifierHash.h"
#include "opt.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

bool isIdentifierCharacterNoBegin[129] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

bool isIdentifierCharacterBegin[129] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
bool inIdentifierCharset(char c, bool begin);

void generateFunctionKeyInfoStreamFastMain(char *source,
                                           char *outputKeyInfoStream,
                                           uint64_t *identifierHashList,
                                           char **userFunctionList,
                                           char **userDefinedFunctionList) {
    char *currentSource = source;

    bool inIdentifier = false;
    char *currentIdentifierBegin = currentSource;
    char *currentIdentifierEnd = currentSource;
    size_t userFunctionLen = 0;
    /*
{
    int i, j, v1, v2, t;
    FILE *fp = fopen("", "");
    */

    while (*currentSource != '\0') {
        if (inIdentifierCharset(*currentSource, !inIdentifier)) {
            if (!inIdentifier) {
                inIdentifier = true;
                currentIdentifierBegin = currentSource;
            }
        } else {
            if (inIdentifier) {
                inIdentifier = false;
                currentIdentifierEnd = currentSource;

                if (!isKeepwordsSlice(currentIdentifierBegin,
                                      currentIdentifierEnd,
                                      identifierHashList)) {
                    if (*currentIdentifierEnd == '(') {
                        bool isUserDefinedFunction = false;

                        char **currentUserDefinedFunction =
                            userDefinedFunctionList;

                        size_t currentIdentifierLen =
                            currentIdentifierEnd - currentIdentifierBegin;
                        while (*currentUserDefinedFunction != NULL) {
                            if (memcmp(currentIdentifierBegin,
                                       *currentUserDefinedFunction,
                                       currentIdentifierLen) == 0) {
                                if ((*currentUserDefinedFunction)
                                        [currentIdentifierLen] == '\0') {
                                    isUserDefinedFunction = true;
                                    break;
                                }
                            }
                            currentUserDefinedFunction++;
                        }

                        if (isUserDefinedFunction) {
                            char *functionName = (char *)malloc(
                                (currentIdentifierLen + 1) * sizeof(char));
                            memcpy(functionName, currentIdentifierBegin,
                                   currentIdentifierLen);

                            functionName[currentIdentifierLen] = '\0';

                            bool userFunctionInserted = false;

                            for (size_t i = 0; i < userFunctionLen; i++) {
                                if (strcmp(userFunctionList[i], functionName) ==
                                    0) {
                                    userFunctionInserted = true;
                                    break;
                                }
                            }

                            if (!userFunctionInserted) {
                                userFunctionList[userFunctionLen] =
                                    functionName;
                                userFunctionLen++;
                            }

                            memcpy(outputKeyInfoStream,
                                   (const void *)USER_FUNCTION_IDENTIFIER,
                                   sizeof(USER_FUNCTION_IDENTIFIER) - 1);

                            outputKeyInfoStream +=
                                sizeof(USER_FUNCTION_IDENTIFIER) - 1;
                        }
                    }

                } else {
                    memcpy(outputKeyInfoStream, currentIdentifierBegin,
                           currentIdentifierEnd - currentIdentifierBegin);
                    outputKeyInfoStream +=
                        currentIdentifierEnd - currentIdentifierBegin;
                }
                // keep currentIdentifierEnd
                currentSource--;
            } else {
                if ((*currentSource != ' ') && (*currentSource != '\n') &&
                    (*currentSource != '\r') && (*currentSource != '\t')) {
                    *outputKeyInfoStream = *currentSource;
                    outputKeyInfoStream++;
                }
            }
        }
        currentSource++;
    }
    *outputKeyInfoStream = '\0';
}

void generateFunctionKeyInfoStreamFast(char *source, char *outputKeyInfoStream,
                                       uint64_t *identifierHashList,
                                       char **userDefinedFunctionList) {
    char *currentSource = source;

    bool inIdentifier = false;
    char *currentIdentifierBegin = currentSource;
    char *currentIdentifierEnd = currentSource;

    /*
{
    int i, j, v1, v2, t;
    FILE *fp = fopen("", "");
    */

    while (*currentSource != '\0') {
        if (inIdentifierCharset(*currentSource, !inIdentifier)) {
            if (!inIdentifier) {
                inIdentifier = true;
                currentIdentifierBegin = currentSource;
            }
        } else {
            if (inIdentifier) {
                inIdentifier = false;
                currentIdentifierEnd = currentSource;

                if (!isKeepwordsSlice(currentIdentifierBegin,
                                      currentIdentifierEnd,
                                      identifierHashList)) {
                    if (*currentIdentifierEnd == '(') {
                        bool isUserDefinedFunction = false;

                        char **currentUserDefinedFunction =
                            userDefinedFunctionList;

                        size_t currentIdentifierLen =
                            currentIdentifierEnd - currentIdentifierBegin;
                        while (*currentUserDefinedFunction != NULL) {
                            if (memcmp(currentIdentifierBegin,
                                       *currentUserDefinedFunction,
                                       currentIdentifierLen) == 0) {
                                if ((*currentUserDefinedFunction)
                                        [currentIdentifierLen] == '\0') {
                                    isUserDefinedFunction = true;
                                    break;
                                }
                            }
                            currentUserDefinedFunction++;
                        }

                        if (isUserDefinedFunction) {
                            char *functionName = (char *)malloc(
                                (currentIdentifierLen + 1) * sizeof(char));
                            memcpy(functionName, currentIdentifierBegin,
                                   currentIdentifierLen);

                            functionName[currentIdentifierLen] = '\0';

                            memcpy(outputKeyInfoStream,
                                   (const void *)USER_FUNCTION_IDENTIFIER,
                                   sizeof(USER_FUNCTION_IDENTIFIER) - 1);

                            outputKeyInfoStream +=
                                sizeof(USER_FUNCTION_IDENTIFIER) - 1;
                        }
                    }

                } else {
                    memcpy(outputKeyInfoStream, currentIdentifierBegin,
                           currentIdentifierEnd - currentIdentifierBegin);
                    outputKeyInfoStream +=
                        currentIdentifierEnd - currentIdentifierBegin;
                }
                // keep currentIdentifierEnd
                currentSource--;
            } else {
                if ((*currentSource != ' ') && (*currentSource != '\n') &&
                    (*currentSource != '\r') && (*currentSource != '\t')) {
                    *outputKeyInfoStream = *currentSource;
                    outputKeyInfoStream++;
                }
            }
        }
        currentSource++;
    }
    *outputKeyInfoStream = '\0';
}

struct ProgramKeyInfo generateProgramKeyInfo(char *source,
                                             uint64_t *identifierHashList) {
    struct ProgramKeyInfo result = {NULL, NULL, 0, NULL};

    result.userFunctionList =
        (char **)malloc(MAX_USER_FUNCTION_COUNT * sizeof(char *));
    // 0 as ending
    memset(result.userFunctionList, 0,
           MAX_USER_FUNCTION_COUNT * sizeof(char *));

    char **userDefinedFunctionList =
        (char **)malloc(MAX_USER_FUNCTION_COUNT * sizeof(char *));
    // 0 as ending
    memset(userDefinedFunctionList, 0,
           MAX_USER_FUNCTION_COUNT * sizeof(char *));
    size_t userDefinedFunctionListLen = 0;

    struct HashTable *table = HashTable_createTable(FUNCTION_HASH_TABLE_MOD);

    result.functionTable = table;

    char *functionName = NULL;
    char *functionBody = NULL;

    char *current = source;

    bool mainFunctionEncountered = false;

    // Find user defined function list
    while (*current != '\f' && *current != '\0') {
        char *currentFunctionBegin = current;
        // functionName
        while (*current != '(') {
            current++;
        }
        size_t functionNameLen = (size_t)(current - currentFunctionBegin);

        functionName = (char *)malloc((functionNameLen + 1) * sizeof(char));
        memcpy(functionName, currentFunctionBegin, functionNameLen);
        functionName[functionNameLen] = '\0';
        userDefinedFunctionList[userDefinedFunctionListLen] = functionName;
        userDefinedFunctionListLen++;
        // paramList
        size_t bracketCount = 0;
        do {
            char *nextOpen = strchr(current, '(');
            char *nextClose = strchr(current, ')');
            if (nextOpen == NULL) {
                bracketCount--;
                current = nextClose + 1;
            } else if (nextClose == NULL) {
                bracketCount++;
                current = nextOpen + 1;
            } else {
                if (nextOpen < nextClose) {
                    bracketCount++;
                    current = nextOpen + 1;
                } else {
                    bracketCount--;
                    current = nextClose + 1;
                }
            }

        } while (bracketCount != 0);

        while (isspace(*current)) {
            current++;
        }

        // body
        bracketCount = 0;
        do {
            char *nextOpen = strchr(current, '{');
            char *nextClose = strchr(current, '}');

            if (nextOpen == NULL) {
                bracketCount--;
                current = nextClose + 1;
            } else if (nextClose == NULL) {
                bracketCount++;
                current = nextOpen + 1;
            } else {
                if (nextOpen < nextClose) {
                    bracketCount++;
                    current = nextOpen + 1;
                } else {
                    bracketCount--;
                    current = nextClose + 1;
                }
            }

        } while (bracketCount != 0);

        while (isspace(*current) && *current != '\f') {
            current++;
        }
    }

    current = source;

    while (*current != '\f' && *current != '\0') {
        char *currentFunctionBegin = current;
        // functionName
        while (*current != '(') {
            current++;
        }
        size_t functionNameLen = (size_t)(current - currentFunctionBegin);

        functionName = (char *)malloc((functionNameLen + 1) * sizeof(char));
        memcpy(functionName, currentFunctionBegin, functionNameLen);
        functionName[functionNameLen] = '\0';

        // paramList
        size_t bracketCount = 0;
        do {
            char *nextOpen = strchr(current, '(');
            char *nextClose = strchr(current, ')');
            if (nextOpen == NULL) {
                bracketCount--;
                current = nextClose + 1;
            } else if (nextClose == NULL) {
                bracketCount++;
                current = nextOpen + 1;
            } else {
                if (nextOpen < nextClose) {
                    bracketCount++;
                    current = nextOpen + 1;
                } else {
                    bracketCount--;
                    current = nextClose + 1;
                }
            }
        } while (bracketCount != 0);

        // body

        while (*current == ' ' || *current == '\n' || *current == '\r' ||
               *current == '\t') {
            current++;
        }

        char *bodyBegin = current;
        bracketCount = 0;
        do {
            char *nextOpen = strchr(current, '{');
            char *nextClose = strchr(current, '}');

            if (nextOpen == NULL) {
                bracketCount--;
                current = nextClose + 1;
            } else if (nextClose == NULL) {
                bracketCount++;
                current = nextOpen + 1;
            } else {
                if (nextOpen < nextClose) {
                    bracketCount++;
                    current = nextOpen + 1;
                } else {
                    bracketCount--;
                    current = nextClose + 1;
                }
            }
        } while (bracketCount != 0);

        size_t bodyLen = (size_t)(current - bodyBegin);

        functionBody = (char *)malloc((bodyLen + 1) * sizeof(char));
        functionBody[bodyLen] = '\0';

        memcpy(functionBody, bodyBegin, (size_t)(current - bodyBegin));

        char *functionKeyInfoStream =
            (char *)malloc((bodyLen + 1) * sizeof(char));
        if (!mainFunctionEncountered && strcmp(functionName, "main") == 0) {
            mainFunctionEncountered = true;

            generateFunctionKeyInfoStreamFastMain(
                functionBody, functionKeyInfoStream, identifierHashList,
                result.userFunctionList, userDefinedFunctionList);

            while (result.userFunctionList[result.userFunctionCount] != 0) {
                result.userFunctionCount++;
            }
        } else {
            generateFunctionKeyInfoStreamFast(
                functionBody, functionKeyInfoStream, identifierHashList,
                userDefinedFunctionList);
        }
        free(functionBody);
        while (*current == '\n' || *current == '\r' || *current == '\t' ||
               *current == ' ') {
            current++;
        }
        HashTable_insert(table, functionName, functionKeyInfoStream);
    }

    for (size_t i = 0; i < userDefinedFunctionListLen; i++) {
        free(userDefinedFunctionList[i]);
    }
    free(userDefinedFunctionList);
    return result;
}

void destroyProgramKeyInfo(struct ProgramKeyInfo *target) {
    HashTable_destroyTable(target->functionTable);

    for (size_t i = 0; i < target->userFunctionCount; i++) {
        free(target->userFunctionList[i]);
    }

    free(target->userFunctionList);
}

size_t generateProgramKeyInfoStreamStr(struct ProgramKeyInfo *programKeyInfo,
                                       char **ptrToResult) {
    size_t outputLen = 0;
    // main
    char *mainInfoStream =
        HashTable_get(programKeyInfo->functionTable, (char *)"main");

    outputLen += strlen(mainInfoStream);
    for (size_t i = 0; i < programKeyInfo->userFunctionCount; i++) {
        char *functionInfoStream = HashTable_get(
            programKeyInfo->functionTable, programKeyInfo->userFunctionList[i]);
        outputLen += strlen(functionInfoStream);
    }

    char *result = (char *)malloc((outputLen + 1) * sizeof(char));
    *ptrToResult = result;

    strcpy(result, mainInfoStream);
    for (size_t i = 0; i < programKeyInfo->userFunctionCount; i++) {
        char *functionInfoStream = HashTable_get(
            programKeyInfo->functionTable, programKeyInfo->userFunctionList[i]);
        strcat(result, functionInfoStream);
    }

    return outputLen;
}

inline bool inIdentifierCharset(char c, bool begin) {
    if (begin) {
        return isIdentifierCharacterBegin[(unsigned)c];
    } else {
        return isIdentifierCharacterNoBegin[(unsigned)c];
    }
}

unsigned long long HashTable_hash(char *data) {
    unsigned long long result = 0;
    while (*data != '\0') {
        result = result * 127 + (unsigned long long)*data;
        data++;
    }
    return result;
}
void HashTable_destroyKey(char *key) { free(key); }
void HashTable_destroyValue(char *value) { free(value); }
bool HashTable_keyEquals(char *a, char *b) {
    if (strcmp(a, b) == 0) {
        return true;
    } else {
        return false;
    }
}
void HashTable_printKey(char *key) { printf("%s", key); }
void HashTable_printValue(char *value) { printf("%s", value); }