#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

#define ENTRY_RESIZE_FACTOR 2
#define ENTRY_INIT_CAPACITY 2

struct HashTable_Item {
    char *key;
    char *value;
};

struct TableEntry {

    union {
        struct HashTable_Item *itemLst;
        struct HashTable_Item item;
    } inner;
    size_t len;
    size_t capacity;
};

struct HashTable {
    struct TableEntry *table;
    size_t capacity;
    size_t len;
    size_t mod;
};

unsigned long long HashTable_hash(char *data);
void HashTable_destroyKey(char *key);
void HashTable_destroyValue(char *value);
bool HashTable_keyEquals(char *a, char *b);
void HashTable_printKey(char *key);
void HashTable_printValue(char *value);

struct HashTable *HashTable_createTable(size_t mod);
void HashTable_destroyTable(struct HashTable *target);
size_t HashTable_getLen(struct HashTable *target);
// Test itemLst -> item and item -> itemLst
bool HashTable_insert(struct HashTable *target, char *key, char *value);
// Test itemLst -> item and item -> itemLst
char *HashTable_get(struct HashTable *target, char *key);
// Test itemLst -> item and item -> itemLst
bool HashTable_delete(struct HashTable *target, char *key);
void HashTable_forEach(struct HashTable *target, void (*f)(char *, char *));

void HashTable_printTable(struct HashTable *target);
#endif
