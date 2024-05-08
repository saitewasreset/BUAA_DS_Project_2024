#ifndef __IDENTIFIER_HASH_H
#define __IDENTIFIER_HASH_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_KEEP_WORDS_LEN 9
#define KEEP_WORDS_LIST_LEN 165
#define IDENTIFIER_HASH_BASE 64

// a * 64 == a << 6

uint64_t identifierHash(char *identifier);
// [begin, end)
uint64_t identifierSliceHash(char *str, size_t begin, size_t end);

void generateKeepwordsList(FILE *src, uint64_t *wordsList);
bool isKeepwords(char *identifier, uint64_t *wordsList);
bool isKeepwordsSlice(char *str, size_t begin, size_t end, uint64_t *wordsList);
#endif