#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

unsigned int hash(const char *name) {
    unsigned int hash = 0;
    while (*name)
        hash = (hash << 5) + *name++;
    return hash % TABLE_SIZE;
}

void initSymbolTable(SymbolTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++)
        table->table[i] = NULL;
}

Symbol* lookup(SymbolTable *table, const char *name) {
    unsigned int index = hash(name);
    Symbol *node = table->table[index];

    while (node) {
        if (strcmp(node->name, name) == 0)
            return node;
        node = node->next;
    }
    return NULL;
}

void insert(SymbolTable *table, const char *name, const char *type, int scope) {
    unsigned int index = hash(name);
    Symbol *newSymbol = malloc(sizeof(Symbol));
    newSymbol->name = my_strdup_st(name);
    newSymbol->type = my_strdup_st(type);
    newSymbol->scope = scope;
    newSymbol->next = NULL;

    if (table->table[index] == NULL) {
        table->table[index] = newSymbol;
        return;
    }

    Symbol *curr = table->table[index];
    while (curr->next != NULL)
        curr = curr->next;

    curr->next = newSymbol;
}

