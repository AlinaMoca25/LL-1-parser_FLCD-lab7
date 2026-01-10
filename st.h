#ifndef ST_H
#define ST_H

#include <stdbool.h>

typedef struct STNode {
    char *lexeme;
    int index;
    
    int bucket;
    int pos;
    struct STNode *next;
} STNode;

typedef struct {
    STNode **buckets;
    int capacity;
    int size;
    
    STNode **index_map;
    int index_map_capacity;
} SymbolTable;

void st_init(SymbolTable *st, int capacity);
void st_free(SymbolTable *st);

int st_put(SymbolTable *st, const char *lexeme);

int st_get(SymbolTable *st, const char *lexeme);

int st_get_location_by_index(SymbolTable *st, int index, int *bucket, int *pos);

void st_dump(SymbolTable *st);

#endif
