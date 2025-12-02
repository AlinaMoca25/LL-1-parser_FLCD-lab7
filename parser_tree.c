// parser_tree.c
// Tree-building parser implementation for Requirement 2

#include "parser_tree.h"
#include "parse_tree.h"
#include "lexer_pif_export.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Tree-building configuration extends base Configuration
typedef struct {
    StrList alpha;      // input stack (w$)
    StrList beta;       // working stack (S$)
    StrList pi;         // output (productions sequence)
    
    // Tree-building additions
    ParseTreeNode **node_stack;  // Stack of nodes being built (parallel to beta)
    int node_stack_count;
    int node_stack_cap;
    
    ParseTreeNode *root;     // Root of parse tree
    
    PIFEntry *pif_entries;  // PIF entries for terminal info
    int pif_count;
    int pif_index;           // Current PIF entry index
} TreeConfiguration;

// Helper: add token to list allowing duplicates
static void add_token_allow_dup(StrList *s, const char *str) {
    if (s->count == s->cap) {
        s->cap *= 2;
        s->items = realloc(s->items, sizeof(char*) * s->cap);
    }
    size_t n = strlen(str) + 1;
    s->items[s->count] = malloc(n);
    memcpy(s->items[s->count], str, n);
    s->count++;
}



// Helper: split input string into tokens from PIF
static StrList split_input_from_pif(PIFEntry *pif_entries, int pif_count, StrList *terms) {
    StrList tokens;
    sl_init(&tokens);
    
    for (int i = 0; i < pif_count; i++) {
        // Map lexeme to terminal name
        const char *terminal = lexeme_to_terminal(pif_entries[i].lexeme);
        if (terminal) {
            sl_add(&tokens, terminal);
        } else {
            // Fallback: use lexeme as-is
            sl_add(&tokens, pif_entries[i].lexeme);
        }
    }
    
    return tokens;
}

// Initialize tree-building configuration
static void tree_config_init(TreeConfiguration *config, PIFEntry *pif_entries, int pif_count,
                             StrList *nonterms, StrList *terms) {
    // Initialize alpha from PIF
    config->alpha = split_input_from_pif(pif_entries, pif_count, terms);
    if (sl_index(&config->alpha, "$") == -1) {
        sl_add(&config->alpha, "$");
    }
    
    // Initialize beta = S$
    sl_init(&config->beta);
    if (nonterms->count > 0) {
        sl_add(&config->beta, nonterms->items[0]); // Start symbol
    }
    sl_add(&config->beta, "$");
    
    // Initialize pi = ε
    sl_init(&config->pi);
    
    // Initialize node stack (parallel to beta)
    config->node_stack_cap = 256;
    config->node_stack = malloc(sizeof(ParseTreeNode*) * config->node_stack_cap);
    config->node_stack_count = 0;
    
    // Create root node for start symbol
    config->root = NULL;
    if (nonterms->count > 0) {
        config->root = tree_node_create(nonterms->items[0], 0);
        config->root->production_index = -1; // Will be set when first production is applied
        config->node_stack[config->node_stack_count++] = config->root;
    }
    
    // Add NULL for $ marker
    config->node_stack[config->node_stack_count++] = NULL;
    
    // Store PIF info
    config->pif_entries = pif_entries;
    config->pif_count = pif_count;
    config->pif_index = 0;
}

// Free tree-building configuration
static void tree_config_free(TreeConfiguration *config) {
    sl_free(&config->alpha);
    sl_free(&config->beta);
    sl_free(&config->pi);
    
    // Free node stack (but not the nodes themselves - they're in the tree)
    free(config->node_stack);
}

// Get head (first element) of a stack (local version)
static const char *stack_head(StrList *stack) {
    if (stack->count == 0) return NULL;
    return stack->items[0];
}

// Remove head from stack (pop from front)
static void pop_head(StrList *stack) {
    if (stack->count == 0) return;
    free(stack->items[0]);
    for (int i = 1; i < stack->count; i++) {
        stack->items[i - 1] = stack->items[i];
    }
    stack->count--;
}

// Push symbol to front of stack
static void push_head(StrList *stack, const char *symbol) {
    if (stack->count == stack->cap) {
        stack->cap *= 2;
        stack->items = realloc(stack->items, sizeof(char*) * stack->cap);
    }
    for (int i = stack->count; i > 0; i--) {
        stack->items[i] = stack->items[i - 1];
    }
    stack->items[0] = malloc(strlen(symbol) + 1);
    strcpy(stack->items[0], symbol);
    stack->count++;
}

// Pop head from node stack
static ParseTreeNode *pop_node_head(TreeConfiguration *config) {
    if (config->node_stack_count == 0) return NULL;
    ParseTreeNode *node = config->node_stack[0];
    for (int i = 1; i < config->node_stack_count; i++) {
        config->node_stack[i - 1] = config->node_stack[i];
    }
    config->node_stack_count--;
    return node;
}

// Push node to front of node stack
static void push_node_head(TreeConfiguration *config, ParseTreeNode *node) {
    if (config->node_stack_count == config->node_stack_cap) {
        config->node_stack_cap *= 2;
        config->node_stack = realloc(config->node_stack, sizeof(ParseTreeNode*) * config->node_stack_cap);
    }
    for (int i = config->node_stack_count; i > 0; i--) {
        config->node_stack[i] = config->node_stack[i - 1];
    }
    config->node_stack[0] = node;
    config->node_stack_count++;
}

// Look up table entry
static int table_lookup(int **table, StrList *nonterms, StrList *terms, const char *stack_top, const char *input_head) {
    int row = -1;
    int col = sl_index(terms, input_head);
    if (col == -1) return PT_ERROR;
    
    int nt_idx = sl_index(nonterms, stack_top);
    if (nt_idx != -1) {
        row = nt_idx;
    } else {
        int term_idx = sl_index(terms, stack_top);
        if (term_idx != -1) {
            row = nonterms->count + term_idx;
        } else {
            return PT_ERROR;
        }
    }
    
    return table[row][col];
}

// Tree-building ActionPush: creates nonterminal node and adds children
static int tree_action_push(TreeConfiguration *config, int **table, StrList *nonterms, StrList *terms, ProdList *prods) {
    const char *A = stack_head(&config->beta);
    const char *u = stack_head(&config->alpha);
    
    if (!A || !u) return 0;
    
    int table_val = table_lookup(table, nonterms, terms, A, u);
    if (table_val < 0) {
        // No valid production - this should not happen if parse table is correct
        return 0;
    }
    if (table_val >= prods->count) {
        // Invalid production index
        return 0;
    }
    
    Production *prod = &prods->items[table_val];
    
    // Safety check: if production RHS is the same as LHS, we have a direct left recursion (shouldn't happen in LL(1))
    // This would cause infinite expansion
    if (prod->rhs_len == 1 && strcmp(prod->rhs[0], A) == 0) {
        return 0; // Direct left recursion detected
    }
    
    // Get the nonterminal node from stack
    // Note: A should be a nonterminal, so A_node should not be NULL
    ParseTreeNode *A_node = pop_node_head(config);
    if (!A_node) {
        // Node stack is out of sync with beta stack - this is an error
        // This can happen if we have a mismatch between node_stack and beta
        return 0;
    }
    
    // Set production index
    A_node->production_index = table_val;
    
    // Pop A from beta
    pop_head(&config->beta);
    
    // Create child nodes for RHS symbols (in reverse order for stack)
    if (prod->rhs_len == 0 || (prod->rhs_len == 1 && (strcmp(prod->rhs[0], "epsilon") == 0 || strcmp(prod->rhs[0], "ε") == 0))) {
        // Epsilon production: no children, no nodes to push
        // A_node remains as a leaf (no children)
    } else {
        // Collect children in reverse order (for stack), then add them in correct order
        ParseTreeNode *children[256];
        int child_count = 0;
        
        // Push RHS in reverse order and create nodes
        for (int i = prod->rhs_len - 1; i >= 0; i--) {
            const char *symbol = prod->rhs[i];
            push_head(&config->beta, symbol);
            
            // Check if symbol is terminal or nonterminal
            int is_term = (sl_index(terms, symbol) != -1);
            
            // Create node for this symbol
            ParseTreeNode *child_node = tree_node_create(symbol, is_term);
            child_node->production_index = -1; // Will be set when production is applied (for nonterminals)
            
            // Store in array (will be reversed later)
            if (child_count < 256) {
                children[child_count++] = child_node;
            }
            
            // Push node to stack (parallel to beta stack)
            push_node_head(config, child_node);
        }
        
        // Add children to A_node in left-to-right order (reverse of what we collected)
        for (int i = child_count - 1; i >= 0; i--) {
            tree_node_add_child(A_node, children[i]);
        }
    }
    
    // Append production index to pi
    char prod_str[32];
    sprintf(prod_str, "%d", table_val);
    add_token_allow_dup(&config->pi, prod_str);
    
    return 1;
}

// Tree-building ActionPop: creates terminal node with PIF info
static int tree_action_pop(TreeConfiguration *config) {
    if (config->alpha.count == 0 || config->beta.count == 0) return 0;
    
    const char *terminal = stack_head(&config->beta);
    ParseTreeNode *term_node = pop_node_head(config);
    
    // Handle NULL node (for $ marker or epsilon)
    if (!term_node) {
        // If it's $, we still need to pop from stacks
        if (strcmp(terminal, "$") == 0) {
            pop_head(&config->alpha);
            pop_head(&config->beta);
            return 1;
        }
        // Otherwise, node stack is out of sync - error
        return 0;
    }
    
    // Associate PIF entry with terminal node (only for real terminals, not $)
    if (strcmp(terminal, "$") != 0 && config->pif_index < config->pif_count) {
        PIFEntry *entry = &config->pif_entries[config->pif_index];
        if (term_node->lexeme) free(term_node->lexeme);
        term_node->lexeme = malloc(strlen(entry->lexeme) + 1);
        strcpy(term_node->lexeme, entry->lexeme);
        term_node->bucket = entry->bucket;
        term_node->pos = entry->pos;
        config->pif_index++;
    } else if (strcmp(terminal, "$") == 0) {
        // For $ marker, set lexeme to "$"
        if (term_node->lexeme) free(term_node->lexeme);
        term_node->lexeme = malloc(2);
        strcpy(term_node->lexeme, "$");
        term_node->bucket = -1;
        term_node->pos = -1;
    }
    
    // Pop from both stacks
    pop_head(&config->alpha);
    pop_head(&config->beta);
    
    return 1;
}

// Main tree-building LL(1) parsing algorithm
ParseTreeOutput ll1_parse_with_tree(const char *input, int **table, 
                                     StrList *nonterms, StrList *terms, 
                                     ProdList *prods, PIFEntry *pif_entries, int pif_count) {
    ParseTreeOutput output;
    output.result = PARSE_ERROR;
    output.tree = NULL;
    output.error_location = NULL;
    
    TreeConfiguration config;
    tree_config_init(&config, pif_entries, pif_count, nonterms, terms);
    
    int go = 1;
    const char *s = NULL;
    int loop_count = 0;
    const int MAX_LOOPS = 10000;  // Safety limit to prevent infinite loops
    int last_beta_count = config.beta.count;
    int no_progress_count = 0;
    
    while (go) {
        loop_count++;
        if (loop_count > MAX_LOOPS) {
            // Infinite loop detected - create detailed error
            go = 0;
            s = "err";
            if (output.error_location) free(output.error_location);
            output.error_location = malloc(512);
            const char *beta_head = stack_head(&config.beta);
            const char *alpha_head = stack_head(&config.alpha);
            sprintf(output.error_location, "Parser loop limit exceeded: stack='%s', input='%s', beta_count=%d, node_count=%d, alpha_count=%d", 
                    beta_head ? beta_head : "NULL", 
                    alpha_head ? alpha_head : "NULL",
                    config.beta.count, config.node_stack_count, config.alpha.count);
            break;
        }
        
        // Check if we're making progress (beta stack should change or alpha should decrease)
        if (config.beta.count == last_beta_count && config.alpha.count > 0) {
            no_progress_count++;
            if (no_progress_count > 100) {
                // No progress for 100 iterations - likely infinite loop
                go = 0;
                s = "err";
                if (output.error_location) free(output.error_location);
                output.error_location = malloc(512);
                const char *beta_head = stack_head(&config.beta);
                const char *alpha_head = stack_head(&config.alpha);
                sprintf(output.error_location, "No progress detected: stack='%s', input='%s'", 
                        beta_head ? beta_head : "NULL", 
                        alpha_head ? alpha_head : "NULL");
                break;
            }
        } else {
            no_progress_count = 0;
            last_beta_count = config.beta.count;
        }
        
        const char *beta_head = stack_head(&config.beta);
        const char *alpha_head = stack_head(&config.alpha);
        
        if (!beta_head || !alpha_head) {
            go = 0;
            s = "err";
            break;
        }
        
        int table_val = table_lookup(table, nonterms, terms, beta_head, alpha_head);
        
        if (table_val >= 0) {
            // Production: Push action
            // Safety check: detect if stack is growing too large (infinite loop)
            if (config.beta.count > 1000) {
                // Stack is too large - likely infinite loop
                go = 0;
                s = "err";
                if (output.error_location) free(output.error_location);
                output.error_location = malloc(256);
                sprintf(output.error_location, "Stack too large (%d): stack='%s', input='%s'", 
                        config.beta.count, beta_head, alpha_head);
                break;
            }
            
            // Check node stack sync
            if (config.node_stack_count != config.beta.count) {
                // Node stack out of sync - this is a bug
                go = 0;
                s = "err";
                if (output.error_location) free(output.error_location);
                output.error_location = malloc(512);
                sprintf(output.error_location, "Node stack out of sync: beta_count=%d, node_count=%d", 
                        config.beta.count, config.node_stack_count);
                break;
            }
            
            if (!tree_action_push(&config, table, nonterms, terms, prods)) {
                go = 0;
                s = "err";
                break;
            }
            
            // Verify node stack is still in sync after push
            if (config.node_stack_count != config.beta.count) {
                go = 0;
                s = "err";
                if (output.error_location) free(output.error_location);
                output.error_location = malloc(512);
                sprintf(output.error_location, "Node stack out of sync after push: beta_count=%d, node_count=%d", 
                        config.beta.count, config.node_stack_count);
                break;
            }
        } else if (table_val == PT_POP) {
            // Pop action
            if (!tree_action_pop(&config)) {
                go = 0;
                s = "err";
                break;
            }
        } else if (table_val == PT_ACCEPT) {
            // Accept
            go = 0;
            s = "acc";
        } else {
            // Error - no valid action in parse table
            go = 0;
            s = "err";
            // Create detailed error message showing what was expected vs what was found
            if (output.error_location) free(output.error_location);
            output.error_location = malloc(512);
            if (beta_head && alpha_head) {
                sprintf(output.error_location, "Parse error: no action for stack='%s', input='%s' (table_val=%d)", 
                        beta_head, alpha_head, table_val);
            } else {
                sprintf(output.error_location, "Parse error: stack=%s, input=%s", 
                        beta_head ? beta_head : "NULL", alpha_head ? alpha_head : "NULL");
            }
        }
    }
    
    if (s && strcmp(s, "acc") == 0) {
        output.result = PARSE_ACCEPT;
        // Root node is tracked separately
        output.tree = config.root;
    } else {
        output.result = PARSE_ERROR;
        // Only set error_location if it wasn't already set (to preserve detailed error message)
        if (!output.error_location) {
            const char *err_loc = stack_head(&config.alpha);
            if (err_loc) {
                output.error_location = malloc(strlen(err_loc) + 1);
                strcpy(output.error_location, err_loc);
            } else {
                output.error_location = malloc(64);
                strcpy(output.error_location, "unknown location");
            }
        }
        // Free tree on error
        if (config.node_stack_count > 0) {
            for (int i = 0; i < config.node_stack_count; i++) {
                if (config.node_stack[i]) {
                    tree_node_free(config.node_stack[i]);
                }
            }
        }
    }
    
    tree_config_free(&config);
    return output;
}

// Free parse tree output
void free_parse_tree_output(ParseTreeOutput *output) {
    if (output->tree) {
        tree_node_free(output->tree);
        output->tree = NULL;
    }
    if (output->error_location) {
        free(output->error_location);
        output->error_location = NULL;
    }
}

