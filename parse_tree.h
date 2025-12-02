// parse_tree.h
// Parse tree structure for Requirement 2

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Parse tree node
typedef struct ParseTreeNode {
    char *symbol;           // Grammar symbol (terminal or nonterminal)
    int is_terminal;         // 1 if terminal, 0 if nonterminal
    int production_index;    // Production index if nonterminal (or -1 for terminal)
    
    // Tree structure
    struct ParseTreeNode *father;      // Parent node
    struct ParseTreeNode *sibling;     // Next sibling (right sibling)
    struct ParseTreeNode *child;        // First child (leftmost child)
    
    // Token info (for terminals)
    char *lexeme;           // Original lexeme from PIF
    int bucket;             // Symbol table bucket
    int pos;                // Symbol table position
} ParseTreeNode;

// Create a new parse tree node
ParseTreeNode *tree_node_create(const char *symbol, int is_terminal);

// Free a parse tree node and its children
void tree_node_free(ParseTreeNode *node);

// Add a child to a node
void tree_node_add_child(ParseTreeNode *parent, ParseTreeNode *child);

// Print parse tree as table (father/sibling relations)
void tree_print_table(ParseTreeNode *root, FILE *out);

// Get node index in a linearized tree (for table output)
int tree_get_node_index(ParseTreeNode *root, ParseTreeNode *node, int *counter);

#endif // PARSE_TREE_H

