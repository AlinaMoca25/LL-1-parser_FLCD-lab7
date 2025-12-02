// parser.c
// LL(1) parser implementation - Step 3: Analyze sequence based on moves between configurations

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper: add token to list allowing duplicates (unlike sl_add which prevents duplicates)
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

// Helper: split input string into tokens (space-separated)
static StrList split_input(const char *input) {
    StrList tokens;
    sl_init(&tokens);
    
    if (!input || strlen(input) == 0) {
        return tokens;
    }
    
    // Manual parsing to avoid strtok issues
    const char *p = input;
    while (*p) {
        // Skip whitespace
        while (*p && (isspace((unsigned char)*p))) p++;
        if (!*p) break;
        
        // Find token end
        const char *start = p;
        while (*p && !isspace((unsigned char)*p)) p++;
        
        // Extract token
        int len = p - start;
        if (len > 0) {
            char *token = malloc(len + 1);
            memcpy(token, start, len);
            token[len] = '\0';
            add_token_allow_dup(&tokens, token); // Allow duplicates for input tokens
            free(token);
        }
    }
    
    return tokens;
}

// Initialize configuration: (w$, S$, ε)
void config_init(Configuration *config, const char *input, StrList *nonterms) {
    // Initialize alpha = w$ (input stack)
    config->alpha = split_input(input);
    // Add $ to alpha if not already present
    if (sl_index(&config->alpha, "$") == -1) {
        sl_add(&config->alpha, "$");
    }
    
    // Initialize beta = S$ (working stack)
    sl_init(&config->beta);
    if (nonterms->count > 0) {
        sl_add(&config->beta, nonterms->items[0]); // Start symbol
    }
    sl_add(&config->beta, "$");
    
    // Initialize pi = ε (output)
    sl_init(&config->pi);
}

// Free configuration resources
void config_free(Configuration *config) {
    sl_free(&config->alpha);
    sl_free(&config->beta);
    sl_free(&config->pi);
}

// Get head (first element) of a stack
const char *head(StrList *stack) {
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
    // Shift all elements right
    for (int i = stack->count; i > 0; i--) {
        stack->items[i] = stack->items[i - 1];
    }
    stack->items[0] = malloc(strlen(symbol) + 1);
    strcpy(stack->items[0], symbol);
    stack->count++;
}

// Look up table entry: returns table value or PT_ERROR
static int table_lookup(int **table, StrList *nonterms, StrList *terms, const char *stack_top, const char *input_head) {
    int row = -1;
    int col = sl_index(terms, input_head);
    if (col == -1) return PT_ERROR;
    
    // Check if stack_top is a nonterminal
    int nt_idx = sl_index(nonterms, stack_top);
    if (nt_idx != -1) {
        row = nt_idx;
    } else {
        // Check if stack_top is a terminal
        int term_idx = sl_index(terms, stack_top);
        if (term_idx != -1) {
            row = nonterms->count + term_idx; // Terminal rows start after nonterminal rows
        } else {
            return PT_ERROR;
        }
    }
    
    return table[row][col];
}

// ActionPush: (ux, Aα$, π) ⊢ (ux, βα$, πi) if M(A, u) = (β, i)
int action_push(Configuration *config, int **table, StrList *nonterms, StrList *terms, ProdList *prods) {
    const char *A = head(&config->beta); // Top of working stack (nonterminal)
    const char *u = head(&config->alpha); // Current input symbol
    
    if (!A || !u) return 0;
    
    int table_val = table_lookup(table, nonterms, terms, A, u);
    
    // Check if it's a production index (>= 0)
    if (table_val < 0) return 0;
    
    // Get the production
    if (table_val >= prods->count) return 0;
    Production *prod = &prods->items[table_val];
    
    // Pop A from beta
    pop_head(&config->beta);
    
    // Push β (RHS of production) onto beta in reverse order
    // If production is epsilon, don't push anything
    if (prod->rhs_len == 0 || (prod->rhs_len == 1 && (strcmp(prod->rhs[0], "epsilon") == 0 || strcmp(prod->rhs[0], "ε") == 0))) {
        // Epsilon production: nothing to push
    } else {
        // Push RHS in reverse order (so first symbol ends up on top)
        for (int i = prod->rhs_len - 1; i >= 0; i--) {
            push_head(&config->beta, prod->rhs[i]);
        }
    }
    
    // Append production index to pi (allow duplicates - same production can be applied multiple times)
    char prod_str[32];
    sprintf(prod_str, "%d", table_val);
    add_token_allow_dup(&config->pi, prod_str);
    
    return 1;
}

// ActionPop: (ux, aα$, π) ⊢ (x, α$, π) if M(a,u)=pop
int action_pop(Configuration *config) {
    // Pop from both stacks
    if (config->alpha.count == 0 || config->beta.count == 0) return 0;
    pop_head(&config->alpha);
    pop_head(&config->beta);
    return 1;
}


// Main LL(1) parsing algorithm
ParseOutput ll1_parse(const char *input, int **table, StrList *nonterms, StrList *terms, ProdList *prods) {
    ParseOutput output;
    output.result = PARSE_ERROR;
    sl_init(&output.productions);
    output.error_location = NULL;
    
    Configuration config;
    config_init(&config, input, nonterms);
    
    int go = 1;
    const char *s = NULL;
    
    while (go) {
        const char *beta_head = head(&config.beta);
        const char *alpha_head = head(&config.alpha);
        
        if (!beta_head || !alpha_head) {
            go = 0;
            s = "err";
            break;
        }
        
        int table_val = table_lookup(table, nonterms, terms, beta_head, alpha_head);
        
        if (table_val >= 0) {
            // Production: Push action
            if (!action_push(&config, table, nonterms, terms, prods)) {
                go = 0;
                s = "err";
                break;
            }
        } else if (table_val == PT_POP) {
            // Pop action
            if (!action_pop(&config)) {
                go = 0;
                s = "err";
                break;
            }
        } else if (table_val == PT_ACCEPT) {
            // Accept
            go = 0;
            s = "acc";
        } else {
            // Error
            go = 0;
            s = "err";
        }
    }
    
    if (s && strcmp(s, "acc") == 0) {
        output.result = PARSE_ACCEPT;
        // Copy pi to output.productions (allow duplicates - same production can be applied multiple times)
        for (int i = 0; i < config.pi.count; i++) {
            add_token_allow_dup(&output.productions, config.pi.items[i]);
        }
    } else {
        output.result = PARSE_ERROR;
        const char *err_loc = head(&config.alpha);
        if (err_loc) {
            output.error_location = malloc(strlen(err_loc) + 1);
            strcpy(output.error_location, err_loc);
        }
    }
    
    config_free(&config);
    return output;
}

// Print parse output
void print_parse_output(ParseOutput *output, ProdList *prods) {
    if (output->result == PARSE_ACCEPT) {
        printf("Sequence accepted\n");
        printf("String of productions: ");
        for (int i = 0; i < output->productions.count; i++) {
                int prod_idx = atoi(output->productions.items[i]);
                if (prod_idx >= 0 && prod_idx < prods->count) {
                    printf("p%d", prod_idx);
                    if (i + 1 < output->productions.count) printf(" ");
                }
        }
        printf("\n");
    } else {
        printf("Sequence not accepted, syntax error at %s\n", 
               output->error_location ? output->error_location : "unknown location");
    }
}

// Free parse output
void free_parse_output(ParseOutput *output) {
    sl_free(&output->productions);
    if (output->error_location) {
        free(output->error_location);
    }
}

