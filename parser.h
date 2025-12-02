// parser.h
// LL(1) parser implementation - Step 3: Analyze sequence based on moves between configurations

#ifndef PARSER_H
#define PARSER_H

#include "first_follow.h"
#include "parse_table.h"

// Configuration structure: (α, β, π)
// α = input stack (remaining input)
// β = working stack (current parsing stack)
// π = output (sequence of productions)
typedef struct {
    StrList alpha;  // input stack (w$)
    StrList beta;   // working stack (S$)
    StrList pi;     // output (productions sequence)
} Configuration;

// Parse result
typedef enum {
    PARSE_ACCEPT,
    PARSE_ERROR
} ParseResult;

typedef struct {
    ParseResult result;
    StrList productions;  // sequence of production indices if accepted
    char *error_location; // token where error occurred if error
} ParseOutput;

// Initialize configuration: (w$, S$, ε)
void config_init(Configuration *config, const char *input, StrList *nonterms);

// Free configuration resources
void config_free(Configuration *config);

// Get head (top) of a stack (first element)
const char *head(StrList *stack);

// ActionPush: (ux, Aα$, π) ⊢ (ux, βα$, πi) if M(A, u) = (β, i)
// Returns 1 on success, 0 on error
int action_push(Configuration *config, int **table, StrList *nonterms, StrList *terms, ProdList *prods);

// ActionPop: (ux, aα$, π) ⊢ (x, α$, π) if M(a,u)=pop
// Returns 1 on success, 0 on error
int action_pop(Configuration *config);

// Main LL(1) parsing algorithm
// Returns ParseOutput with result and productions/error info
ParseOutput ll1_parse(const char *input, int **table, StrList *nonterms, StrList *terms, ProdList *prods);

// Main LL(1) parsing algorithm with verbose debug output
ParseOutput ll1_parse_verbose(const char *input, int **table, StrList *nonterms, StrList *terms, ProdList *prods);

// Print parse output
void print_parse_output(ParseOutput *output, ProdList *prods);

// Free parse output
void free_parse_output(ParseOutput *output);

#endif // PARSER_H

