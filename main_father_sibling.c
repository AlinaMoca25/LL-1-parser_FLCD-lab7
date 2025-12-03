// main_father_sibling.c
// Build parse table, parse PIF tokens and construct father-sibling parse tree

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "first_follow.h"
#include "parse_table.h"
#include "parser.h"

// PIF type copy (match lexer.l)
typedef struct {
    char token[257];
    int bucket;
    int offset;
} PIF;

// Portable strdup replacement
static char *my_strdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *r = (char*)malloc(n);
    if (r) memcpy(r, s, n);
    return r;
}

// Externs provided by the lexer (if linked)
extern PIF ProgramInternalForm[];
extern int pifLength;

// Stub definitions for when not linked with lexer
PIF ProgramInternalForm[300];
int pifLength = 0;

// Tree node for father-sibling representation
typedef struct {
    int id;
    char *symbol;
    int parent;       // parent node id or -1
    int left_child;   // id of leftmost child or -1
    int right_sibling; // id of right sibling or -1
} FSNode;

typedef struct {
    FSNode *nodes;
    int count;
    int cap;
} FSList;

static void fs_init(FSList *l){ l->count=0; l->cap=128; l->nodes=malloc(sizeof(FSNode)*l->cap); }
static void fs_free(FSList *l){ for(int i=0;i<l->count;i++) free(l->nodes[i].symbol); free(l->nodes); }
static int fs_add(FSList *l, const char *sym, int parent){ if(l->count==l->cap){ l->cap*=2; l->nodes=realloc(l->nodes,sizeof(FSNode)*l->cap); } FSNode *n=&l->nodes[l->count]; n->id=l->count; n->symbol=my_strdup(sym); n->parent=parent; n->left_child=-1; n->right_sibling=-1; l->count++; return n->id; }

// Utility: build input string from PIF tokens
static char *build_input_from_pif(){
    if(pifLength <= 0) return NULL;
    // estimate length
    int len = 0; for(int i=0;i<pifLength;i++) len += (int)strlen(ProgramInternalForm[i].token) + 1;
    char *s = malloc(len+4);
    s[0]=0;
    for(int i=0;i<pifLength;i++){
        if(i) strcat(s, " ");
        strcat(s, ProgramInternalForm[i].token);
    }
    return s;
}

int main(int argc, char **argv){
    const char *grammar = "grammar.ll1";
    if(argc>=2) grammar = argv[1];

    StrList nonterms; sl_init(&nonterms);
    StrList terms; sl_init(&terms);
    ProdList prods; pl_init(&prods);

    load_grammar(grammar, &nonterms, &terms, &prods);

    // ensure $ in terms
    if(sl_index(&terms, "$") == -1) sl_add(&terms, "$" );

    FirstTable first; compute_first(&nonterms, &terms, &prods, &first);
    FollowTable follow; compute_follow(&nonterms, &terms, &prods, &first, &follow);

    int **table = build_parse_table(&nonterms, &terms, &prods, &first, &follow);
    if(!table){ fprintf(stderr, "Failed to build parse table\n"); return 1; }

    // Build input from PIF (if available), from a provided PIF file, or run external lexer on example file
    char *input = NULL;
    // Priority 1: in-process PIF filled by linked lexer
    if(pifLength > 0){
        input = build_input_from_pif();
    } else if(argc >= 3 && strcmp(argv[2], "--pif") == 0){
        // Usage: program grammar.ll1 --pif piffile
        if(argc < 4){ fprintf(stderr, "Usage: %s <grammar> --pif <piffile>\n", argv[0]); return 1; }
        const char *pifFile = argv[3];
        FILE *fp = fopen(pifFile, "r");
        if(!fp){ perror("fopen piffile"); return 1; }
        char buf[512];
        int cap = 1024; int len = 0;
        input = malloc(cap);
        input[0] = '\0';
        while(fgets(buf, sizeof(buf), fp)){
            char *p = strstr(buf, "Token:");
            if(p){
                p += strlen("Token:");
                while(*p && isspace((unsigned char)*p)) p++;
                char *q = p;
                while(*q && !isspace((unsigned char)*q)) q++;
                int tlen = q - p;
                if(tlen>0){
                    if(len + tlen + 2 > cap){ cap *= 2; input = realloc(input, cap); }
                    if(len>0){ input[len++] = ' '; input[len] = '\0'; }
                    strncat(input, p, tlen);
                    len += tlen;
                }
            }
        }
        fclose(fp);
    } else if(argc >= 3){
        // Try to run external lexer.exe on the provided example file and parse its PIF output
        const char *exampleFile = argv[2];
        char tmpPif[] = "pif_tmp.txt";
        char cmd[1024];
        // lexer.exe should print the Program Internal Form via showProgramInternalForm
        snprintf(cmd, sizeof(cmd), "lexer.exe %s > %s", exampleFile, tmpPif);
        int rc = system(cmd);
        if(rc == 0){
            // parse tmpPif for lines starting with "Token:"
            FILE *fp = fopen(tmpPif, "r");
            if(fp){
                char buf[512];
                // build token string
                int cap = 1024; int len = 0;
                input = malloc(cap);
                input[0] = '\0';
                while(fgets(buf, sizeof(buf), fp)){
                    char *p = strstr(buf, "Token:");
                    if(p){
                        // move to token
                        p += strlen("Token:");
                        while(*p && isspace((unsigned char)*p)) p++;
                        // token up to whitespace
                        char *q = p;
                        while(*q && !isspace((unsigned char)*q)) q++;
                        int tlen = q - p;
                        if(tlen>0){
                            if(len + tlen + 2 > cap){ cap *= 2; input = realloc(input, cap); }
                            if(len>0){ input[len++] = ' '; input[len] = '\0'; }
                            strncat(input, p, tlen);
                            len += tlen;
                        }
                    }
                }
                fclose(fp);
                // remove temp file
                remove(tmpPif);
            }
        }
        if(!input){
            // fallback: treat argv[2] as space-separated token list
            input = my_strdup(argv[2]);
        }
    } else {
        fprintf(stderr, "No input sequence provided and PIF empty.\n");
        return 1;
    }

    printf("Parsing input: %s\n", input);

    ParseOutput out = ll1_parse(input, table, &nonterms, &terms, &prods);
    if(out.result != PARSE_ACCEPT){
        printf("Parsing failed at %s\n", out.error_location ? out.error_location : "unknown");
        free(input);
        free_parse_output(&out);
        return 1;
    }

    // Build father-sibling tree from production sequence
    FSList tree; fs_init(&tree);
    // create root node = start symbol
    int root = fs_add(&tree, nonterms.items[0], -1);

    // frontier: list of node ids representing current left-to-right sequence
    int *frontier = malloc(sizeof(int)*1024);
    int fcap = 1024; int fcount = 0;
    frontier[fcount++] = root;

    // apply productions in order
    for(int pi=0; pi<out.productions.count; pi++){
        int prod_idx = atoi(out.productions.items[pi]);
        if(prod_idx < 0 || prod_idx >= prods.count) continue;
        Production *pr = &prods.items[prod_idx];

        // find leftmost frontier node whose symbol equals LHS
        int found_pos = -1;
        for(int i=0;i<fcount;i++){
            int nid = frontier[i];
            if(strcmp(tree.nodes[nid].symbol, nonterms.items[pr->lhs])==0){ found_pos = i; break; }
        }
        if(found_pos == -1){ continue; }

        int parent_id = frontier[found_pos];

        // create children for RHS
        int first_child_id = -1;
        int prev_child_id = -1;
        if(pr->rhs_len==0 || (pr->rhs_len==1 && (strcmp(pr->rhs[0], "epsilon")==0 || strcmp(pr->rhs[0], "ε")==0))){
            // epsilon: no children; remove node from frontier
            // set left_child = -1
            tree.nodes[parent_id].left_child = -1;
            // remove frontier element
            for(int k=found_pos+1;k<fcount;k++) frontier[k-1]=frontier[k]; fcount--; continue;
        }
        for(int r=0;r<pr->rhs_len;r++){
            const char *sym = pr->rhs[r];
            int cid = fs_add(&tree, sym, parent_id);
            if(first_child_id == -1) first_child_id = cid;
            if(prev_child_id != -1) tree.nodes[prev_child_id].right_sibling = cid;
            prev_child_id = cid;
        }
        tree.nodes[parent_id].left_child = first_child_id;

        // replace frontier element at found_pos with the sequence of child ids
        // ensure capacity
        int need = pr->rhs_len - 1; // new elements to shift
        if(fcount + need > fcap){ fcap = (fcount+need)*2; frontier = realloc(frontier, sizeof(int)*fcap); }
        // shift tail right
        for(int k=fcount-1;k>found_pos;k--) frontier[k+need]=frontier[k];
        // insert children
        for(int r=0;r<pr->rhs_len;r++) frontier[found_pos + r] = tree.nodes[first_child_id + r].id;
        fcount += need;
    }

    // print father-sibling table
    printf("\nFather-Sibling Parse Tree (NodeID | Symbol | Parent | LeftChild | RightSibling)\n");
    for(int i=0;i<tree.count;i++){
        printf("%d | %s | %d | %d | %d\n", tree.nodes[i].id, tree.nodes[i].symbol, tree.nodes[i].parent, tree.nodes[i].left_child, tree.nodes[i].right_sibling);
    }

    // cleanup
    fs_free(&tree);
    free(frontier);
    free(input);
    free_parse_output(&out);

    // free table
    int rows = nonterms.count + terms.count;
    for(int i=0;i<rows;i++) free(table[i]); free(table);

    for(int i=0;i<nonterms.count;i++) sl_free(&first.sets[i]); free(first.sets);
    for(int i=0;i<nonterms.count;i++) sl_free(&follow.sets[i]); free(follow.sets);

    sl_free(&nonterms); sl_free(&terms); pl_free(&prods);

    return 0;
}
