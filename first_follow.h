// first_follow.h
// Declarations for FIRST/FOLLOW helper (implementation expected in first_follow.c)

#ifndef FIRST_FOLLOW_H
#define FIRST_FOLLOW_H

#include <stddef.h>

#define INITIAL_CAP 32

typedef struct {
    char **items;
    int count;
    int cap;
} StrList;

typedef struct {
    int lhs; // index into nonterminals
    int rhs_len;
    char **rhs; // tokens
} Production;

typedef struct {
    Production *items;
    int count;
    int cap;
} ProdList;

typedef struct {
    StrList *sets; // parallel array, length = number of nonterminals
} FirstTable;

typedef FirstTable FollowTable;

// Grammar loader: fills nonterms, terms and prods
void load_grammar(const char *path, StrList *nonterms, StrList *terms, ProdList *prods);

// Compute FIRST and FOLLOW tables for given grammar
void compute_first(StrList *nonterms, StrList *terms, ProdList *prods, FirstTable *ft);
void compute_follow(StrList *nonterms, StrList *terms, ProdList *prods, FirstTable *first, FollowTable *follow);

// Helpers for StrList and ProdList (implementations may exist in first_follow.c)
void sl_init(StrList *s);
void sl_free(StrList *s);
void sl_add(StrList *s, const char *str);
int sl_index(StrList *s, const char *str);

void pl_init(ProdList *p);
void pl_free(ProdList *p);

#endif // FIRST_FOLLOW_H
