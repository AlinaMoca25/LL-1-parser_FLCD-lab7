#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t st_hash(const char *s) {
    uint64_t h = 1469598103934665603ULL;
    while (*s) {
        h ^= (uint8_t)(*s++);
        h *= 1099511628211ULL;
    }
    return h;
}

static char *dupstr(const char *s) {
    size_t n = strlen(s);
    char *p = (char*)malloc(n+1);
    memcpy(p, s, n+1);
    return p;
}

void st_init(SymbolTable *st, int capacity) {
    st->capacity = capacity;
    st->size = 0;
    st->buckets = (STNode**)calloc(capacity, sizeof(STNode*));
    st->index_map_capacity = capacity > 0 ? capacity : 16;
    st->index_map = (STNode**)calloc(st->index_map_capacity, sizeof(STNode*));
}

void st_free(SymbolTable *st) {
    for (int i=0;i<st->capacity;i++) {
        STNode *cur = st->buckets[i];
        while (cur) { STNode *nx = cur->next; free(cur->lexeme); free(cur); cur = nx; }
    }
    free(st->buckets);
    st->buckets = NULL;
    free(st->index_map);
    st->index_map = NULL;
}

static inline int bucket_index(SymbolTable *st, const char *lexeme) {
    uint64_t h1 = st_hash(lexeme);
    int h = (int)(h1 % (uint64_t)st->capacity);
    return h;
}

int st_get(SymbolTable *st, const char *lexeme) {
    int b = bucket_index(st, lexeme);
    STNode *cur = st->buckets[b];
    while (cur) {
        if (strcmp(cur->lexeme, lexeme)==0) return cur->index;
        cur = cur->next;
    }
    return -1;
}

int st_put(SymbolTable *st, const char *lexeme) {
    int idx = st_get(st, lexeme);
    if (idx != -1) return idx;
    int h = bucket_index(st, lexeme);

    STNode *node = (STNode*)malloc(sizeof(STNode));
    node->lexeme = dupstr(lexeme);
    node->index = st->size;
    node->bucket = h;
    node->next = NULL;

    STNode *cur = st->buckets[h];
    int pos = 0;
    if (!cur) {
        st->buckets[h] = node;
        pos = 0;
    } else {
        pos = 0;
        while (cur->next) { cur = cur->next; pos++; }
        
        cur->next = node;
        pos++;
    }
    node->pos = pos;

    if (st->size >= st->index_map_capacity) {
        int newcap = st->index_map_capacity * 2;
        if (newcap <= st->size) newcap = st->size + 16;
        STNode **m = (STNode**)realloc(st->index_map, sizeof(STNode*) * newcap);
        if (m) {
            for (int i = st->index_map_capacity; i < newcap; ++i) m[i] = NULL;
            st->index_map = m;
            st->index_map_capacity = newcap;
        }
    }

    if (st->index_map && st->size < st->index_map_capacity) st->index_map[st->size] = node;

    st->size++;
    return node->index;
}

int st_get_location_by_index(SymbolTable *st, int index, int *bucket, int *pos) {
    if (index < 0 || index >= st->size) return -1;
    if (!st->index_map) {
        
        for (int b = 0; b < st->capacity; ++b) {
            STNode *cur = st->buckets[b];
            int p = 0;
            while (cur) {
                if (cur->index == index) {
                    if (bucket) *bucket = b;
                    if (pos) *pos = p;
                    return 0;
                }
                cur = cur->next; p++;
            }
        }
        return -1;
    }
    STNode *n = st->index_map[index];
    if (!n) return -1;
    if (bucket) *bucket = n->bucket;
    if (pos) *pos = n->pos;
    return 0;
}

void st_dump(SymbolTable *st) {
    printf("~~~~ Symbol Table (hash) size=%d cap=%d ~~~~\n", st->size, st->capacity);
    for (int i=0;i<st->capacity;i++) {
        STNode *cur = st->buckets[i];
        if (!cur) continue;
        printf("[%d] -> ", i);
        
        while (cur) { printf("(\"%s\", idx=%d, bucket=%d, pos=%d) ", cur->lexeme, cur->index, cur->bucket, cur->pos); cur = cur->next; }
        printf("\n");
    }
    printf("~~~~~~~~ End ST ~~~~~~~~\n\n");
}
