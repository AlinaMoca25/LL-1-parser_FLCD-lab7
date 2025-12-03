#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define TABLE_SIZE 211

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

#endif
