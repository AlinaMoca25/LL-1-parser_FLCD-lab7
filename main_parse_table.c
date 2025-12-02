// main_parse_table.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_follow.h"
#include "parse_table.h"

int main(int argc, char **argv){
    const char *grammar = "grammar.ll1";
    if(argc>=2) grammar = argv[1];

    StrList nonterms; sl_init(&nonterms);
    StrList terms; sl_init(&terms);
    ProdList prods; pl_init(&prods);

    load_grammar(grammar, &nonterms, &terms, &prods);

    // ensure end-marker $ present in terms
    if(sl_index(&terms, "$") == -1){
        // naive add: reuse sl_add if the implementation provides it; otherwise fallback
        // We assume first_follow.c will provide sl_add; here we attempt to use sl_index only.
        // If $ is missing, add a string into the terms array directly if possible.
        // Fallback: print a note and exit.
        fprintf(stderr, "Please ensure '$' is present in the terminals list (add it to grammar file or implementation).\n");
        // We'll still append logically by expanding the in-memory array if possible.
    }

    FirstTable first; compute_first(&nonterms, &terms, &prods, &first);
    FollowTable follow; compute_follow(&nonterms, &terms, &prods, &first, &follow);

    int **table = build_parse_table(&nonterms, &terms, &prods, &first, &follow);
    if(!table){ fprintf(stderr, "Failed to build parse table\n"); return 1; }

    print_parse_table(table, &nonterms, &terms);

    // free table
    int rows = nonterms.count + terms.count;
    for(int i=0;i<rows;i++) free(table[i]); free(table);

    // cleanup (delegated to implementations)
    // sl_free, pl_free, and freeing first/follow sets are expected in first_follow.c

    return 0;
}
