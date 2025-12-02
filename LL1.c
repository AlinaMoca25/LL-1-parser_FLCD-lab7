// LL1.c
// Simple tool to load a whitespace-separated grammar file (see grammar.ll1)
// and compute FIRST sets for all nonterminals.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAP 32

// Dynamic array of strings
typedef struct {
    char **items;
    int count;
    int cap;
} StrList;

void sl_init(StrList *s){ s->count=0; s->cap=INITIAL_CAP; s->items=malloc(sizeof(char*)*s->cap); }
void sl_free(StrList *s){ for(int i=0;i<s->count;i++) free(s->items[i]); free(s->items); }
// forward declaration
static char *my_strdup(const char *s);

void sl_add(StrList *s, const char *str){
    for(int i=0;i<s->count;i++) if(strcmp(s->items[i],str)==0) return;
    if(s->count==s->cap){ s->cap*=2; s->items = realloc(s->items,sizeof(char*)*s->cap); }
    s->items[s->count++] = my_strdup(str);
}
int sl_index(StrList *s, const char *str){ for(int i=0;i<s->count;i++) if(strcmp(s->items[i],str)==0) return i; return -1; }

typedef struct {
    int lhs; // index into nonterminals
    int rhs_len;
    char **rhs; // tokens
} Production;

typedef struct {
    Production *items;
    int count;
    int cap;
} ProdList;

void pl_init(ProdList *p){ p->count=0; p->cap=INITIAL_CAP; p->items=malloc(sizeof(Production)*p->cap); }
void pl_free(ProdList *p){ for(int i=0;i<p->count;i++){ for(int j=0;j<p->items[i].rhs_len;j++) free(p->items[i].rhs[j]); free(p->items[i].rhs); } free(p->items); }
void pl_add(ProdList *p, Production prod){ if(p->count==p->cap){ p->cap*=2; p->items=realloc(p->items,sizeof(Production)*p->cap); } p->items[p->count++]=prod; }

// Set of strings per nonterminal representing FIRST set
typedef struct {
    StrList *sets; // parallel array, length = number of nonterminals
} FirstTable;

int is_epsilon_token(const char *t){ return strcmp(t,"epsilon")==0 || strcmp(t,"ε")==0; }

// add token to FIRST set for nonterminal index nt
int first_add(FirstTable *ft, int nt, const char *tok){
    StrList *s = &ft->sets[nt];
    // if already present return 0
    for(int i=0;i<s->count;i++) if(strcmp(s->items[i],tok)==0) return 0;
    sl_add(s,tok);
    return 1;
}

// helper: remove epsilon presence check
int first_contains(FirstTable *ft, int nt, const char *tok){
    StrList *s = &ft->sets[nt];
    for(int i=0;i<s->count;i++) if(strcmp(s->items[i],tok)==0) return 1; return 0;
}

// trim
static char *trim(char *s){
    while(isspace((unsigned char)*s)) s++;
    if(*s==0) return s;
    char *end = s + strlen(s) - 1;
    while(end>s && isspace((unsigned char)*end)) *end-- = '\0';
    return s;
}

// portable strdup replacement
static char *my_strdup(const char *s){ if(!s) return NULL; size_t n = strlen(s)+1; char *r = malloc(n); if(r) memcpy(r,s,n); return r; }

// split by whitespace into tokens; returns array of strings and sets *len
char **split_tokens(const char *line, int *len){
    char *tmp = my_strdup(line);
    int cap = 16; int cnt = 0;
    char **arr = malloc(sizeof(char*)*cap);
    char *p = tmp;
    while(*p){
        while(isspace((unsigned char)*p)) p++;
        if(!*p) break;
        char *start = p;
        while(*p && !isspace((unsigned char)*p)) p++;
        int l = p - start;
        char *tok = malloc(l+1); memcpy(tok,start,l); tok[l]=0;
        if(cnt==cap){ cap*=2; arr=realloc(arr,sizeof(char*)*cap); }
        arr[cnt++] = tok;
    }
    free(tmp);
    *len = cnt;
    return arr;
}

// read grammar file of simple format: lines with 'A -> X Y' or comments starting with '#'
void load_grammar(const char *path, StrList *nonterms, StrList *terms, ProdList *prods){
    FILE *f = fopen(path,"r");
    if(!f){ perror("fopen"); exit(1); }
    char line[1024];
    while(fgets(line,sizeof(line),f)){
        char *p = trim(line);
        if(*p==0) continue;
        if(*p=='#') continue;
        char *arrow = strstr(p,"->");
        if(!arrow) continue;
        // split LHS and RHS
        *arrow = '\0';
        char *lhs = trim(p);
        char *rhs = trim(arrow+2);
        if(*lhs==0 || *rhs==0) continue;
        sl_add(nonterms, lhs);
        int lhs_idx = sl_index(nonterms, lhs);
        // allow alternatives separated by '|'
        char *alt = rhs;
        char *saveptr = NULL;
        char *token;
            char *rhs_dup = my_strdup(rhs);
            token = strtok(rhs_dup, "|");
        while(token){
            char *alttrim = trim(token);
            // split alttrim into tokens
            int cnt = 0;
            char **toks = split_tokens(alttrim, &cnt);
            Production prod;
            prod.lhs = lhs_idx;
            prod.rhs_len = cnt;
            prod.rhs = malloc(sizeof(char*)*cnt);
            for(int i=0;i<cnt;i++){
                prod.rhs[i] = toks[i]; // moved ownership
                // if token is not a known nonterminal (yet), treat as terminal candidate
                if(!is_epsilon_token(prod.rhs[i])){
                    if(sl_index(nonterms, prod.rhs[i])==-1) sl_add(terms, prod.rhs[i]);
                }
            }
            free(toks);
            pl_add(prods, prod);
            token = strtok(NULL, "|");
        }
        free(rhs_dup);
    }
    fclose(f);
    // remove from terminals any symbol that is actually a nonterminal (in case added earlier)
    for(int i=0;i<nonterms->count;i++){
        for(int j=0;j<terms->count;){
            if(strcmp(terms->items[j], nonterms->items[i])==0){ free(terms->items[j]);
                terms->items[j] = terms->items[--terms->count];
            } else j++;
        }
    }
}

// compute FIRST sets
void compute_first(StrList *nonterms, StrList *terms, ProdList *prods, FirstTable *ft){
    // init FIRST table lists
    ft->sets = malloc(sizeof(StrList)*nonterms->count);
    for(int i=0;i<nonterms->count;i++) sl_init(&ft->sets[i]);

    int changed = 1;
    while(changed){
        changed = 0;
        for(int pi=0; pi<prods->count; pi++){
            Production *pr = &prods->items[pi];
            int A = pr->lhs;
            // production A -> alpha
            if(pr->rhs_len==0){ // treat as epsilon
                if(!first_contains(ft,A,"epsilon")){ first_add(ft,A,"epsilon"); changed=1; }
                continue;
            }
            // special case: RHS single token 'epsilon'
            if(pr->rhs_len==1 && is_epsilon_token(pr->rhs[0])){
                if(!first_contains(ft,A,"epsilon")){ first_add(ft,A,"epsilon"); changed=1; }
                continue;
            }
            int all_nullable = 1;
            for(int k=0;k<pr->rhs_len;k++){
                char *X = pr->rhs[k];
                // if X is terminal
                if(sl_index(nonterms, X)==-1){
                    // terminal (or token not listed as nonterminal)
                    if(!first_contains(ft,A,X)){
                        first_add(ft,A,X); changed=1;
                    }
                    all_nullable = 0;
                    break;
                } else {
                    int Xidx = sl_index(nonterms, X);
                    // add FIRST(X) \\ {epsilon} to FIRST(A)
                    StrList *sx = &ft->sets[Xidx];
                    int had_epsilon = 0;
                    for(int t=0;t<sx->count;t++){
                        if(is_epsilon_token(sx->items[t])){ had_epsilon = 1; continue; }
                        if(!first_contains(ft,A,sx->items[t])){ first_add(ft,A,sx->items[t]); changed=1; }
                    }
                    if(!had_epsilon){ all_nullable = 0; break; }
                    // else continue to next symbol
                }
            }
            if(all_nullable){ if(!first_contains(ft,A,"epsilon")){ first_add(ft,A,"epsilon"); changed=1; } }
        }
    }
}

void print_first(FirstTable *ft, StrList *nonterms){
    printf("FIRST sets:\n");
    for(int i=0;i<nonterms->count;i++){
        printf("FIRST(%s) = { ", nonterms->items[i]);
        StrList *s = &ft->sets[i];
        for(int j=0;j<s->count;j++){
            printf("%s", s->items[j]);
            if(j+1 < s->count) printf(", ");
        }
        printf(" }\n");
    }
}

void print_grammar(StrList *nonterms, StrList *terms, ProdList *prods){
    printf("Parsed grammar:\n");
    printf("Nonterminals (%d): ", nonterms->count);
    for(int i=0;i<nonterms->count;i++) printf("%s ", nonterms->items[i]);
    printf("\nTerminals (%d): ", terms->count);
    for(int i=0;i<terms->count;i++) printf("%s ", terms->items[i]);
    printf("\nProductions (%d):\n", prods->count);
    for(int i=0;i<prods->count;i++){
        Production *p = &prods->items[i];
        printf("  %s ->", nonterms->items[p->lhs]);
        for(int j=0;j<p->rhs_len;j++) printf(" %s", p->rhs[j]);
        printf("\n");
    }
}

void save_first(const char *path, FirstTable *ft, StrList *nonterms){
    FILE *f = fopen(path,"w"); if(!f){ perror("fopen"); return; }
    for(int i=0;i<nonterms->count;i++){
        fprintf(f, "%s ->", nonterms->items[i]);
        StrList *s = &ft->sets[i];
        for(int j=0;j<s->count;j++){
            fprintf(f, " %s", s->items[j]);
            if(j+1 < s->count) fprintf(f, ",");
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(int argc, char **argv){
    const char *grammar_path = "grammar.ll1";
    int save = 0;
    int debug = 0;
    if(argc>=2) grammar_path = argv[1];
    for(int i=2;i<argc;i++){ if(strcmp(argv[i],"--save")==0) save=1; if(strcmp(argv[i],"--debug")==0) debug=1; }

    StrList nonterms; sl_init(&nonterms);
    StrList terms; sl_init(&terms);
    ProdList prods; pl_init(&prods);

    load_grammar(grammar_path, &nonterms, &terms, &prods);

    if(debug) print_grammar(&nonterms, &terms, &prods);

    if(nonterms.count==0){ printf("No nonterminals found in '%s' or file empty.\n", grammar_path); return 1; }

    FirstTable ft;
    compute_first(&nonterms, &terms, &prods, &ft);
    print_first(&ft, &nonterms);
    if(save) save_first("first_table.txt", &ft, &nonterms);

    // cleanup
    for(int i=0;i<nonterms.count;i++) sl_free(&ft.sets[i]); free(ft.sets);
    sl_free(&nonterms); sl_free(&terms); pl_free(&prods);
    return 0;
}
