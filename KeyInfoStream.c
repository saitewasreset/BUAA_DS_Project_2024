#include "KeyInfoStream.h"
#include "HashTableFast.h"
#include "IdentifierHash.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

bool inIdentifierCharset(char c, bool begin);

void generateFunctionKeyInfoStream(char *source, char *keyInfoStream,
                                   uint64_t *identifierHashList,
                                   char **userFunctionList,
                                   bool saveUserFunction,
                                   char **userDefinedFunctionList) {
    size_t sourceLen = strlen(source);
    char *_source = (char *)malloc((sourceLen + 1) * sizeof(char));
    strcpy(_source, source);

    bool inIdentifier = false;
    size_t currentIdentifierBegin = 0;
    size_t currentIdentifierEnd = 0;

    size_t userFunctionListLen = 0;

    // identifier: [begin, end)
    // 0x00 for to delete identifier; 0xFF for user function name
    for (size_t i = 0; i < sourceLen; i++) {
        if (inIdentifierCharset(_source[i], !inIdentifier)) {
            if (!inIdentifier) {
                inIdentifier = true;
                currentIdentifierBegin = i;
            }
        } else {
            if (inIdentifier) {
                inIdentifier = false;
                currentIdentifierEnd = i;

                if (!isKeepwordsSlice(_source, currentIdentifierBegin,
                                      currentIdentifierEnd,
                                      identifierHashList)) {
                    // None-keep words identifier + ( -> function call ?
                    if (_source[currentIdentifierEnd] != '(') {
                        // delete
                        memset(_source + currentIdentifierBegin, 0,
                               (currentIdentifierEnd - currentIdentifierBegin) *
                                   sizeof(char));
                    } else {
                        // neither keep words nor user defined function ->
                        // delete?

                        char *userFunctionName =
                            (char *)malloc((currentIdentifierEnd -
                                            currentIdentifierBegin + 1) *
                                           sizeof(char));
                        memcpy(userFunctionName,
                               _source + currentIdentifierBegin,
                               (currentIdentifierEnd - currentIdentifierBegin) *
                                   sizeof(char));
                        userFunctionName[currentIdentifierEnd -
                                         currentIdentifierBegin] = '\0';

                        bool isUserDefinedFunction = false;
                        size_t userDefinedFunctionIdx = 0;
                        while (
                            userDefinedFunctionList[userDefinedFunctionIdx] !=
                            0) {
                            if (strcmp(userFunctionName,
                                       userDefinedFunctionList
                                           [userDefinedFunctionIdx]) == 0) {
                                isUserDefinedFunction = true;
                                break;
                            }
                            userDefinedFunctionIdx++;
                        }
                        if (isUserDefinedFunction) {
                            // replace with "FUNC"
                            if (saveUserFunction) {
                                // check exists (slow ?)
                                bool userFunctionFound = false;

                                for (size_t j = 0; j < userFunctionListLen;
                                     j++) {
                                    if (strcmp(userFunctionName,
                                               userFunctionList[j]) == 0) {
                                        userFunctionFound = true;

                                        break;
                                    }
                                }

                                if (!userFunctionFound) {
                                    userFunctionList[userFunctionListLen] =
                                        userFunctionName;
                                    userFunctionListLen++;
                                } else {
                                    free(userFunctionName);
                                }
                            } else {
                                free(userFunctionName);
                            }
                            memset(_source + currentIdentifierBegin, 0xFF,
                                   (currentIdentifierEnd -
                                    currentIdentifierBegin) *
                                       sizeof(char));
                        } else {
                            memset(_source + currentIdentifierBegin, 0,
                                   (currentIdentifierEnd -
                                    currentIdentifierBegin) *
                                       sizeof(char));
                            free(userFunctionName);
                        }
                    }
                }
            }
        }
    }

    size_t keyInfoStreamIndex = 0;
    bool userFunctionIdentifierInserted = false;
    for (size_t i = 0; i < sourceLen; i++) {

        if (((uint8_t)_source[i]) != 0 && ((uint8_t)_source[i]) != 0xFF) {
            userFunctionIdentifierInserted = false;
            if (_source[i] == ' ' || _source[i] == '\n' || _source[i] == '\r' ||
                _source[i] == '\t') {
                continue;
            }
            keyInfoStream[keyInfoStreamIndex] = _source[i];
            keyInfoStreamIndex++;
        } else if (((uint8_t)_source[i]) == 0) {
            userFunctionIdentifierInserted = false;
        } else {
            if (!userFunctionIdentifierInserted) {
                memcpy(keyInfoStream + keyInfoStreamIndex,
                       USER_FUNCTION_IDENTIFIER,
                       sizeof(USER_FUNCTION_IDENTIFIER) - 1);
                keyInfoStreamIndex += sizeof(USER_FUNCTION_IDENTIFIER) - 1;
                userFunctionIdentifierInserted = true;
            }
        }
    }

    keyInfoStream[keyInfoStreamIndex] = '\0';
    free(_source);
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
            if (*current == '(') {
                bracketCount++;
            } else if (*current == ')') {
                bracketCount--;
            }
            current++;
        } while (bracketCount != 0);

        while (*current == '\n' || *current == '\r' || *current == '\t' ||
               *current == ' ') {
            current++;
        }

        // body
        bracketCount = 0;
        do {
            if (*current == '{') {
                bracketCount++;
            } else if (*current == '}') {
                bracketCount--;
            }
            current++;
        } while (bracketCount != 0);

        while (*current == '\n' || *current == '\r' || *current == '\t' ||
               *current == ' ') {
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
            if (*current == '(') {
                bracketCount++;
            } else if (*current == ')') {
                bracketCount--;
            }
            current++;
        } while (bracketCount != 0);

        // body

        while (*current == '\n' || *current == '\r' || *current == '\t' ||
               *current == ' ') {
            current++;
        }

        char *bodyBegin = current;
        bracketCount = 0;
        do {
            if (*current == '{') {
                bracketCount++;
            } else if (*current == '}') {
                bracketCount--;
            }
            current++;
        } while (bracketCount != 0);

        size_t bodyLen = (size_t)(current - bodyBegin);

        functionBody = (char *)malloc((bodyLen + 1) * sizeof(char));
        functionBody[bodyLen] = '\0';

        memcpy(functionBody, bodyBegin, (size_t)(current - bodyBegin));

        char *functionKeyInfoStream =
            (char *)malloc((bodyLen + 1) * sizeof(char));
        if (!mainFunctionEncountered && strcmp(functionName, "main") == 0) {
            mainFunctionEncountered = true;
            generateFunctionKeyInfoStream(
                functionBody, functionKeyInfoStream, identifierHashList,
                result.userFunctionList, true, userDefinedFunctionList);
            while (result.userFunctionList[result.userFunctionCount] != 0) {
                result.userFunctionCount++;
            }
        } else {
            generateFunctionKeyInfoStream(
                functionBody, functionKeyInfoStream, identifierHashList,
                result.userFunctionList, false, userDefinedFunctionList);
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
    if (isalpha(c)) {
        return true;
    }

    if (isalnum(c)) {
        if (begin) {
            return false;
        } else {
            return true;
        }
    }

    if (c == '_') {
        return true;
    }

    return false;
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