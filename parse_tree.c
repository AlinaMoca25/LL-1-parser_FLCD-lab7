// parse_tree.c
// Parse tree implementation for Requirement 2

#include "parse_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ParseTreeNode *tree_node_create(const char *symbol, int is_terminal) {
    ParseTreeNode *node = malloc(sizeof(ParseTreeNode));
    if (!node) return NULL;
    
    node->symbol = malloc(strlen(symbol) + 1);
    strcpy(node->symbol, symbol);
    node->is_terminal = is_terminal;
    node->production_index = -1;
    
    node->father = NULL;
    node->sibling = NULL;
    node->child = NULL;
    
    node->lexeme = NULL;
    node->bucket = -1;
    node->pos = -1;
    
    return node;
}

void tree_node_free(ParseTreeNode *node) {
    if (!node) return;
    
    // Free children recursively
    ParseTreeNode *child = node->child;
    while (child) {
        ParseTreeNode *next = child->sibling;
        tree_node_free(child);
        child = next;
    }
    
    // Free node data
    free(node->symbol);
    if (node->lexeme) free(node->lexeme);
    free(node);
}

void tree_node_add_child(ParseTreeNode *parent, ParseTreeNode *child) {
    if (!parent || !child) return;
    
    child->father = parent;
    
    if (!parent->child) {
        parent->child = child;
    } else {
        // Add as rightmost sibling
        ParseTreeNode *sibling = parent->child;
        while (sibling->sibling) {
            sibling = sibling->sibling;
        }
        sibling->sibling = child;
    }
}

// Helper: collect all nodes in pre-order
static void collect_nodes(ParseTreeNode *node, ParseTreeNode **nodes, int *count, int max) {
    if (!node || *count >= max) return;
    
    nodes[(*count)++] = node;
    
    ParseTreeNode *child = node->child;
    while (child) {
        collect_nodes(child, nodes, count, max);
        child = child->sibling;
    }
}

void tree_print_table(ParseTreeNode *root, FILE *out) {
    if (!root) return;
    
    // Collect all nodes
    ParseTreeNode **nodes = malloc(sizeof(ParseTreeNode*) * 10000);
    int node_count = 0;
    collect_nodes(root, nodes, &node_count, 10000);
    
    // Print header
    fprintf(out, "Index | Symbol | Type | Production | Father | Sibling | Lexeme | ST Location\n");
    fprintf(out, "------|--------|------|------------|--------|---------|--------|------------\n");
    
    // Print each node
    for (int i = 0; i < node_count; i++) {
        ParseTreeNode *node = nodes[i];
        
        // Find node index
        int node_idx = i;
        
        // Find father index
        int father_idx = -1;
        if (node->father) {
            for (int j = 0; j < node_count; j++) {
                if (nodes[j] == node->father) {
                    father_idx = j;
                    break;
                }
            }
        }
        
        // Find sibling index
        int sibling_idx = -1;
        if (node->sibling) {
            for (int j = 0; j < node_count; j++) {
                if (nodes[j] == node->sibling) {
                    sibling_idx = j;
                    break;
                }
            }
        }
        
        fprintf(out, "%5d | %-6s | %-4s | %10d | %6d | %7d | %-6s | ",
                node_idx,
                node->symbol,
                node->is_terminal ? "TERM" : "NTERM",
                node->production_index,
                father_idx,
                sibling_idx,
                node->lexeme ? node->lexeme : "-");
        
        if (node->bucket >= 0 && node->pos >= 0) {
            fprintf(out, "%d,%d", node->bucket, node->pos);
        } else {
            fprintf(out, "-");
        }
        fprintf(out, "\n");
    }
    
    free(nodes);
}

int tree_get_node_index(ParseTreeNode *root, ParseTreeNode *node, int *counter) {
    if (!root || !node || !counter) return -1;
    
    if (root == node) return (*counter)++;
    
    int idx = -1;
    ParseTreeNode *child = root->child;
    while (child) {
        idx = tree_get_node_index(child, node, counter);
        if (idx >= 0) return idx;
        child = child->sibling;
    }
    
    return -1;
}

