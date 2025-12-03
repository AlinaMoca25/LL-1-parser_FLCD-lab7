#ifndef PARSER_TREE_H
#define PARSER_TREE_H

#include "parser.h"
#include "parse_tree.h"
#include "pif_reader.h"

// Parse output with tree
typedef struct {
    ParseResult result;
    ParseTreeNode *tree;        // Root of parse tree
    char *error_location;
} ParseTreeOutput;

// Parse with tree building
ParseTreeOutput ll1_parse_with_tree(const char *input, int **table, 
                                     StrList *nonterms, StrList *terms, 
                                     ProdList *prods, PIFEntry *pif_entries, int pif_count);

// Free parse tree output
void free_parse_tree_output(ParseTreeOutput *output);

#endif // PARSER_TREE_H

