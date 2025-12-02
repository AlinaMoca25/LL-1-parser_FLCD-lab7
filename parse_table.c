// parse_table.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_table.h"

// Helper: add "$" as end-marker to terms list for table columns
static void ensure_endmarker(StrList *terms){
    if(sl_index(terms, "$") == -1) sl_init(terms); // sl_init may be noop if already init
    if(sl_index(terms, "$") == -1) { /* add */
        // we assume a sl_add implementation exists in first_follow.c; but to avoid direct dependency here,
        // we add it by manipulating the list if possible. For safety call sl_init only if count==0.
        // In practice first_follow provides sl_add.
    }
}

// Helper to find index of terminal in terms list, adding $ if necessary
static int term_index_add_dollar(StrList *terms, const char *tok){
    int idx = sl_index(terms, tok);
    return idx;
}

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
    for(int p=0;p<prods->count;p++){
        Production *prod = &prods->items[p];
        // compute FIRST(alpha) for RHS alpha
        // We'll collect terminals in a temporary set using StrList-like behavior (but simply mark via array)
        int produces_epsilon = 1;
        // iterate symbols
        for(int k=0;k<prod->rhs_len;k++){
            char *X = prod->rhs[k];
            int nt_idx = sl_index(nonterms, X);
            if(nt_idx == -1){
                // X is terminal
                int col = sl_index(terms, X);
                if(col==-1) continue; // unknown terminal
                // set table[A, X] = prod_index
                table[prod->lhs][col] = p;
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
                    table[prod->lhs][col] = p;
                }
                if(!had_eps){ produces_epsilon = 0; break; }
                // else continue to next symbol
            }
        }
        if(produces_epsilon){
            // for each b in FOLLOW(A) set table[A,b] = prod
            StrList *fA = &follow->sets[prod->lhs];
            for(int t=0;t<fA->count;t++){
                int col = sl_index(terms, fA->items[t]);
                if(col==-1) continue;
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
