// parse_table.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_table.h"

int **build_parse_table(StrList *nonterms, StrList *terms, ProdList *prods, FirstTable *first, FollowTable *follow){
    // we'll require that terms contains all terminals but possibly not the end-marker `$`.
    // Add a virtual `$` at the end for columns if not present. Since we don't know sl_add signature here,
    // assume the provided terms already contains `$`. If not, caller should add it.

    int N = nonterms->count;
    int T = terms->count; // assume includes $
    if(T==0){ fprintf(stderr, "No terminals found for parse table\n"); return NULL; }

    // allocate table rows: first N rows for nonterminals, then T rows for terminal behavior (pop/accept)
    int rows = N + T;
    int cols = T;
    int **table = malloc(sizeof(int*)*rows);
    for(int i=0;i<rows;i++){
        table[i] = malloc(sizeof(int)*cols);
        for(int j=0;j<cols;j++) table[i][j] = PT_ERROR;
    }

    // For terminal rows: where top-of-stack is terminal t_i
    // table[row=N + i][col=i] = PT_POP ; for endmarker row, if col is endmarker set ACCEPT
    for(int i=0;i<T;i++){
        int row = N + i;
        for(int j=0;j<cols;j++) table[row][j] = PT_ERROR;
        // match terminal with same lookahead -> pop
        table[N + i][i] = PT_POP;
    }

    // if there's a $ terminal, set its row,col to ACCEPT
    int dollar_idx = sl_index(terms, "$");
    if(dollar_idx != -1){
        table[N + dollar_idx][dollar_idx] = PT_ACCEPT;
    }

    // For nonterminal rows: fill according to algorithm
    // First pass: process productions that start with terminals (higher priority)
    for(int p=0;p<prods->count;p++){
        Production *prod = &prods->items[p];
        if(prod->rhs_len == 0) continue;
        
        // Check if production starts with a terminal
        char *first_sym = prod->rhs[0];
        int nt_idx = sl_index(nonterms, first_sym);
        if(nt_idx == -1){
            // Starts with terminal - process this first (higher priority)
            int col = sl_index(terms, first_sym);
            if(col != -1){
                // Only set if not already set (preserve first match)
                if(table[prod->lhs][col] == PT_ERROR){
                    table[prod->lhs][col] = p;
                }
            }
        }
    }
    
    // Second pass: process all productions normally
    for(int p=0;p<prods->count;p++){
        Production *prod = &prods->items[p];
        // compute FIRST(alpha) for RHS alpha
        // Check if this is an epsilon production first
        int is_epsilon_prod = (prod->rhs_len == 0 || (prod->rhs_len == 1 && (strcmp(prod->rhs[0], "epsilon") == 0 || strcmp(prod->rhs[0], "ε") == 0)));
        
        if(is_epsilon_prod){
            // Epsilon production: set entries for all terminals in FOLLOW(A)
            StrList *fA = &follow->sets[prod->lhs];
            for(int t=0;t<fA->count;t++){
                int col = sl_index(terms, fA->items[t]);
                if(col==-1) continue;
                // Always set epsilon production entries
                table[prod->lhs][col] = p;
            }
            continue; // Skip to next production
        }
        
        // Non-epsilon production: compute FIRST(alpha)
        int produces_epsilon = 1;
        // iterate symbols
        for(int k=0;k<prod->rhs_len;k++){
            char *X = prod->rhs[k];
            int nt_idx = sl_index(nonterms, X);
            if(nt_idx == -1){
                // X is terminal (or epsilon token)
                if(strcmp(X, "epsilon") == 0 || strcmp(X, "ε") == 0){
                    // Epsilon token - continue to next symbol (or end, making produces_epsilon = 1)
                    continue;
                }
                int col = sl_index(terms, X);
                if(col==-1) continue; // unknown terminal
                // set table[A, X] = prod_index (only if not already set by terminal-first pass)
                if(table[prod->lhs][col] == PT_ERROR){
                    table[prod->lhs][col] = p;
                }
                produces_epsilon = 0;
                break;
            } else {
                // X is nonterminal: add FIRST(X) \ {epsilon}
                StrList *sx = &first->sets[nt_idx];
                int had_eps = 0;
                for(int t=0;t<sx->count;t++){
                    if(strcmp(sx->items[t], "epsilon")==0) { had_eps = 1; continue; }
                    int col = sl_index(terms, sx->items[t]);
                    if(col==-1) continue;
                    // Only set if not already set (preserve terminal-first matches)
                    if(table[prod->lhs][col] == PT_ERROR){
                        table[prod->lhs][col] = p;
                    }
                }
                if(!had_eps){ produces_epsilon = 0; break; }
                // else continue to next symbol
            }
        }
        if(produces_epsilon){
            // for each b in FOLLOW(A) set table[A,b] = prod
            // Epsilon productions should always be set (even if there's a conflict)
            StrList *fA = &follow->sets[prod->lhs];
            for(int t=0;t<fA->count;t++){
                int col = sl_index(terms, fA->items[t]);
                if(col==-1) continue;
                // Always set epsilon production entries (don't check PT_ERROR)
                table[prod->lhs][col] = p;
            }
        }
    }

    return table;
}

void print_parse_table(int **table, StrList *nonterms, StrList *terms){
    int N = nonterms->count;
    int T = terms->count;
    int rows = N + T;
    int cols = T;

    // header
    printf("Parse table (rows: nonterminals then terminals; cols: terminals including $)\n");
    printf("\t");
    for(int j=0;j<cols;j++) printf("%s\t", terms->items[j]);
    printf("\n");

    for(int i=0;i<rows;i++){
        if(i < N) printf("%s\t", nonterms->items[i]); else printf("%s\t", terms->items[i - N]);
        for(int j=0;j<cols;j++){
            int v = table[i][j];
            if(v == PT_ERROR) printf("-\t");
            else if(v == PT_POP) printf("pop\t");
            else if(v == PT_ACCEPT) printf("accept\t");
            else printf("p%d\t", v);
        }
        printf("\n");
    }
}
