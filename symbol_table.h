#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#define fileno _fileno
#endif

#define TABLE_SIZE 100

// Portable strdup for Windows
static inline char *my_strdup_st(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *r = (char*)malloc(n);
    if (r) memcpy(r, s, n);
    return r;
}

typedef struct Symbol {
    char *name;
    char *type;
    int scope;
    struct Symbol *next;
} Symbol;

typedef struct {
    Symbol *table[TABLE_SIZE];
} SymbolTable;

typedef struct {
    int bucket;
    int offset;
} STPosition;

unsigned int hash(const char *name);
void initSymbolTable(SymbolTable *table);
Symbol* lookup(SymbolTable *table, const char *name);
void insert(SymbolTable *table, const char *name, const char *type, int scope);

#endif // SYMBOL_TABLE_H
