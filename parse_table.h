// parse_table.h
#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H

#include "first_follow.h"

// Table cell values
#define PT_ERROR   -1
#define PT_PROD     0 // base index; actual stored are prod_index (>=0)
#define PT_POP     -2
#define PT_ACCEPT  -3

// Build parse table. Returns a dynamically allocated 2D array: rows = nonterminals+terminals, cols = terminals (including $).
// Caller must free returned pointer and the nested arrays.
int **build_parse_table(StrList *nonterms, StrList *terms, ProdList *prods, FirstTable *first, FollowTable *follow);

// Print the parse table (includes terminal rows for pop/accept entries)
void print_parse_table(int **table, StrList *nonterms, StrList *terms);

#endif // PARSE_TABLE_H
