#include "IdentifierHash.h"
#include <stdint.h>
#include <stdlib.h>

const uint64_t identifierCharsetMap[128] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  3,  4,  5,  6,  7,  8,  9,  10,
    11, 0,  0,  0,  0,  0,  0,  0,  12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 0,  0,  0,  0,
    1,  0,  38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
    55, 56, 57, 58, 59, 60, 61, 62, 63, 0,  0,  0,  0,  0};

int hashCmp(const void *a, const void *b);

uint64_t identifierHash(char *identifier) {
    uint64_t result = 0;
    uint_fast8_t i = 0;
    while (*identifier != '\0') {
        if (i > 9) {
            return 0;
        }
        result = (result << 6) + identifierCharsetMap[(unsigned)*identifier];
        i++;
        identifier++;
    }
    return result;
}

uint64_t identifierSliceHash(char *str, size_t begin, size_t end) {
    uint64_t result = 0;
    if (end - begin > 9) {
        return 0;
    }

    str += begin;

    for (size_t i = 0; i < (end - begin); i++) {
        result = (result << 6) + identifierCharsetMap[(unsigned)*str];
        str++;
    }

    return result;
}

void generateKeepwordsList(FILE *src, uint64_t *wordsList) {
    char keepwordsBuffer[MAX_KEEP_WORDS_LEN + 1] = {0};
    uint_fast8_t i = 0;
    while (fscanf(src, "%s", keepwordsBuffer) > 0) {
        wordsList[i] = identifierHash(keepwordsBuffer);
        i++;
    }

    qsort(wordsList, KEEP_WORDS_LIST_LEN, sizeof(uint64_t), hashCmp);
}

bool isKeepwords(char *identifier, uint64_t *wordsList) {
    uint64_t hash = identifierHash(identifier);
    if (hash == 0) {
        return false;
    }

    uint64_t *result = bsearch(&hash, wordsList, KEEP_WORDS_LIST_LEN,
                               sizeof(uint64_t), hashCmp);
    if (result == NULL) {
        return false;
    } else {
        return true;
    }
}

bool isKeepwordsSlice(char *str, size_t begin, size_t end,
                      uint64_t *wordsList) {
    uint64_t hash = identifierSliceHash(str, begin, end);
    if (hash == 0) {
        return false;
    }

    uint64_t *result = bsearch(&hash, wordsList, KEEP_WORDS_LIST_LEN,
                               sizeof(uint64_t), hashCmp);
    if (result == NULL) {
        return false;
    } else {
        return true;
    }
}

int hashCmp(const void *a, const void *b) {
    uint64_t uA = *((uint64_t *)a);
    uint64_t uB = *((uint64_t *)b);

    if (uA < uB) {
        return -1;
    } else if (uA > uB) {
        return 1;
    } else {
        return 0;
    }
}