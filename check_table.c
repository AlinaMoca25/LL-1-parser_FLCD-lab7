#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_follow.h"
#include "parse_table.h"

void print_entry(int **table, StrList *nonterms, StrList *terms, const char *nt, const char *t){
    int nt_idx = sl_index(nonterms, nt);
    int t_idx = sl_index(terms, t);
    if(nt_idx==-1){ printf("Nonterminal '%s' not found\n", nt); return; }
    if(t_idx==-1){ printf("Terminal '%s' not found\n", t); return; }
    int rows = nonterms->count + terms->count;
    int cols = terms->count;
    int val = table[nt_idx][t_idx];
    printf("M(%s, %s) = ", nt, t);
    if(val == PT_ERROR) printf("PT_ERROR\n");
    else if(val == PT_POP) printf("PT_POP\n");
    else if(val == PT_ACCEPT) printf("PT_ACCEPT\n");
    else printf("prod p%d\n", val);
}

int main(int argc, char **argv){
    const char *grammar = "grammar.ll1";
    if(argc>=2) grammar = argv[1];

    StrList nonterms; sl_init(&nonterms);
    StrList terms; sl_init(&terms);
    ProdList prods; pl_init(&prods);

    load_grammar(grammar, &nonterms, &terms, &prods);
    if(sl_index(&terms, "$") == -1) sl_add(&terms, "$");

    FirstTable first; compute_first(&nonterms, &terms, &prods, &first);
    FollowTable follow; compute_follow(&nonterms, &terms, &prods, &first, &follow);

    printf("--- FIRST sets ---\n");
    for(int i=0;i<nonterms.count;i++){
        printf("FIRST(%s) = { ", nonterms.items[i]);
        StrList *s = &first.sets[i];
        for(int j=0;j<s->count;j++){ printf("%s", s->items[j]); if(j+1 < s->count) printf(", "); }
        printf(" }\n");
    }
    printf("--- FOLLOW sets ---\n");
    for(int i=0;i<nonterms.count;i++){
        printf("FOLLOW(%s) = { ", nonterms.items[i]);
        StrList *s = &follow.sets[i];
        for(int j=0;j<s->count;j++){ printf("%s", s->items[j]); if(j+1 < s->count) printf(", "); }
        printf(" }\n");
    }

    int **table = build_parse_table(&nonterms, &terms, &prods, &first, &follow);
    if(!table){ fprintf(stderr, "Failed to build parse table\n"); return 1; }

    print_entry(table, &nonterms, &terms, "pipe_expr_tail", "PIPELINE");
    print_entry(table, &nonterms, &terms, "pipe_expr_tail", "KEEP");
    print_entry(table, &nonterms, &terms, "program_tail", "PIPELINE");
    print_entry(table, &nonterms, &terms, "stmt", "PIPELINE");

    print_entry(table, &nonterms, &terms, "primary", "IDENTIFIER");
    print_entry(table, &nonterms, &terms, "id", "LAMBDA");
    print_entry(table, &nonterms, &terms, "lambda", "IDENTIFIER");

        // Print production p28 for inspection
        int p = 28;
        if(p >= 0 && p < prods.count){
            Production *pr = &prods.items[p];
            printf("prod p%d: %s ->", p, nonterms.items[pr->lhs]);
            for(int k=0;k<pr->rhs_len;k++) printf(" %s", pr->rhs[k]);
            printf("\n");
        }

        int pt_idx = sl_index(&nonterms, "program_tail");
        if(pt_idx != -1){
            printf("Productions for program_tail:\n");
            for(int i=0;i<prods.count;i++){
                if(prods.items[i].lhs == pt_idx){
                    printf(" p%d: program_tail ->", i);
                    for(int k=0;k<prods.items[i].rhs_len;k++) printf(" %s", prods.items[i].rhs[k]);
                    printf("\n");
                }
            }
        }

    // cleanup
    for(int i=0;i<nonterms.count;i++){ sl_free(&first.sets[i]); sl_free(&follow.sets[i]); }
    free(first.sets); free(follow.sets);
    for(int i=0;i<nonterms.count + terms.count;i++) free(table[i]); free(table);
    sl_free(&nonterms); sl_free(&terms); pl_free(&prods);

    return 0;
}
