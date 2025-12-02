// main_tree_parser.c
// Main program for Requirement 2: Parse tree generation from PIF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_follow.h"
#include "parse_table.h"
#include "parser_tree.h"
#include "pif_reader.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <grammar_file> <pif_file> [output_file]\n", argv[0]);
        fprintf(stderr, "  grammar_file: LL(1) grammar file\n");
        fprintf(stderr, "  pif_file: PIF (Program Internal Form) file\n");
        fprintf(stderr, "  output_file: (optional) output file for parse tree table\n");
        return 1;
    }
    
    const char *grammar_file = argv[1];
    const char *pif_file = argv[2];
    const char *output_file = (argc >= 4) ? argv[3] : NULL;
    
    // Load grammar
    StrList nonterms, terms;
    ProdList prods;
    sl_init(&nonterms);
    sl_init(&terms);
    pl_init(&prods);
    
    printf("Loading grammar from %s...\n", grammar_file);
    load_grammar(grammar_file, &nonterms, &terms, &prods);
    
    if (nonterms.count == 0 || terms.count == 0 || prods.count == 0) {
        fprintf(stderr, "Error: Failed to load grammar\n");
        return 1;
    }
    
    // Ensure end-marker $ is present in terms (required for parse table)
    if (sl_index(&terms, "$") == -1) {
        sl_add(&terms, "$");
    }
    
    printf("Grammar loaded: %d nonterminals, %d terminals, %d productions\n", 
           nonterms.count, terms.count, prods.count);
    
    // Compute FIRST and FOLLOW
    printf("Computing FIRST and FOLLOW sets...\n");
    FirstTable first;
    first.sets = malloc(sizeof(StrList) * nonterms.count);
    for (int i = 0; i < nonterms.count; i++) {
        sl_init(&first.sets[i]);
    }
    compute_first(&nonterms, &terms, &prods, &first);
    
    FollowTable follow;
    follow.sets = malloc(sizeof(StrList) * nonterms.count);
    for (int i = 0; i < nonterms.count; i++) {
        sl_init(&follow.sets[i]);
    }
    compute_follow(&nonterms, &terms, &prods, &first, &follow);
    
    // Build parse table
    printf("Building parse table...\n");
    int **table = build_parse_table(&nonterms, &terms, &prods, &first, &follow);
    if (!table) {
        fprintf(stderr, "Error: Failed to build parse table\n");
        return 1;
    }
    
    // Read PIF
    printf("Reading PIF from %s...\n", pif_file);
    PIFEntry *pif_entries = NULL;
    int pif_count = 0;
    
    int result = read_pif_from_file(pif_file, &pif_entries, &pif_count);
    if (result < 0) {
        fprintf(stderr, "Error: Failed to read PIF file '%s'\n", pif_file);
        fprintf(stderr, "Make sure the file exists and is in the correct format.\n");
        fprintf(stderr, "Expected format:\n");
        fprintf(stderr, "  ~~~~ Program Internal Form (PIF) ~~~~\n");
        fprintf(stderr, "  lexeme           bucket,pos\n");
        fprintf(stderr, "  lexeme           -1\n");
        fprintf(stderr, "  ~~~~~~~~ End PIF ~~~~~~~~\n");
        return 1;
    }
    
    if (pif_count == 0) {
        fprintf(stderr, "Warning: PIF file contains no entries\n");
        return 1;
    }
    
    printf("PIF loaded: %d entries\n", pif_count);
    
    // Convert PIF to input string (for compatibility, though we'll use PIF directly)
    // Actually, we'll pass PIF entries directly to the parser
    const char *input = ""; // Not used, parser uses PIF directly
    
    // Parse with tree building
    printf("Parsing with tree building...\n");
    ParseTreeOutput parse_output = ll1_parse_with_tree(input, table, &nonterms, &terms, &prods, 
                                                        pif_entries, pif_count);
    
    // Open output file or use stdout
    FILE *out = stdout;
    if (output_file) {
        out = fopen(output_file, "w");
        if (!out) {
            fprintf(stderr, "Error: Failed to open output file %s\n", output_file);
            out = stdout;
        }
    }
    
    // Print results
    if (parse_output.result == PARSE_ACCEPT) {
        fprintf(out, "Sequence accepted\n\n");
        fprintf(out, "Parse Tree (Father/Sibling Relations):\n");
        fprintf(out, "========================================\n\n");
        
        if (parse_output.tree) {
            tree_print_table(parse_output.tree, out);
        } else {
            fprintf(out, "Error: Parse tree is NULL\n");
        }
    } else {
        fprintf(out, "Sequence not accepted\n");
        if (parse_output.error_location) {
            fprintf(out, "Syntax error at: %s\n", parse_output.error_location);
        } else {
            fprintf(out, "Syntax error at: unknown location\n");
        }
        fprintf(stderr, "Parse failed. Error: %s\n", 
                parse_output.error_location ? parse_output.error_location : "unknown");
    }
    
    if (out != stdout) {
        fclose(out);
        printf("Parse tree table written to %s\n", output_file);
    }
    
    // Cleanup
    free_parse_tree_output(&parse_output);
    free_pif_entries(pif_entries, pif_count);
    
    // Free parse table
    for (int i = 0; i < nonterms.count + terms.count; i++) {
        free(table[i]);
    }
    free(table);
    
    // Free FIRST and FOLLOW tables
    for (int i = 0; i < nonterms.count; i++) {
        sl_free(&first.sets[i]);
        sl_free(&follow.sets[i]);
    }
    free(first.sets);
    free(follow.sets);
    
    // Free grammar
    sl_free(&nonterms);
    sl_free(&terms);
    pl_free(&prods);
    
    return (parse_output.result == PARSE_ACCEPT) ? 0 : 1;
}

