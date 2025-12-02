// main_parser.c
// Main program for LL(1) parser - Step 3: Analyze sequence based on moves between configurations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_follow.h"
#include "parse_table.h"
#include "parser.h"

// Helper to add $ to terms if not present
static void ensure_dollar_terminal(StrList *terms) {
    if (sl_index(terms, "$") == -1) {
        sl_add(terms, "$");
    }
}

int main(int argc, char **argv) {
    const char *grammar = "grammar.ll1";
    const char *input_sequence = NULL;
    
    if (argc >= 2) grammar = argv[1];
    if (argc >= 3) input_sequence = argv[2];
    
    // If no input sequence provided, prompt or use default
    if (!input_sequence) {
        fprintf(stderr, "Usage: %s <grammar_file> <input_sequence>\n", argv[0]);
        fprintf(stderr, "Example: %s grammar.ll1 \"a b\"\n", argv[0]);
        fprintf(stderr, "Or: %s grammar.ll1 \"c d\"\n", argv[0]);
        return 1;
    }
    
    StrList nonterms;
    sl_init(&nonterms);
    StrList terms;
    sl_init(&terms);
    ProdList prods;
    pl_init(&prods);
    
    load_grammar(grammar, &nonterms, &terms, &prods);
    
    // Ensure $ is in terms for parse table
    ensure_dollar_terminal(&terms);
    
    // Compute FIRST and FOLLOW
    FirstTable first;
    compute_first(&nonterms, &terms, &prods, &first);
    FollowTable follow;
    compute_follow(&nonterms, &terms, &prods, &first, &follow);
    
    // Build parse table
    int **table = build_parse_table(&nonterms, &terms, &prods, &first, &follow);
    if (!table) {
        fprintf(stderr, "Failed to build parse table\n");
        return 1;
    }
    
    // Print parse table (optional, for debugging)
    // print_parse_table(table, &nonterms, &terms);
    
    // Parse the input sequence
    printf("Parsing input sequence: %s\n", input_sequence);
    printf("---\n");
    
    ParseOutput output = ll1_parse(input_sequence, table, &nonterms, &terms, &prods);
    print_parse_output(&output, &prods);
    
    // Cleanup
    free_parse_output(&output);
    
    // Free table
    int rows = nonterms.count + terms.count;
    for (int i = 0; i < rows; i++) {
        free(table[i]);
    }
    free(table);
    
    // Free FIRST and FOLLOW sets
    for (int i = 0; i < nonterms.count; i++) {
        sl_free(&first.sets[i]);
    }
    free(first.sets);
    
    for (int i = 0; i < nonterms.count; i++) {
        sl_free(&follow.sets[i]);
    }
    free(follow.sets);
    
    // Free grammar data
    sl_free(&nonterms);
    sl_free(&terms);
    pl_free(&prods);
    
    return 0;
}

