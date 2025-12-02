// first_follow.c
// Implementation of grammar loader, FIRST and FOLLOW computation.

#include "first_follow.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *my_strdup(const char *s){ if(!s) return NULL; size_t n = strlen(s)+1; char *r = malloc(n); if(r) memcpy(r,s,n); return r; }

void sl_init(StrList *s){ s->count=0; s->cap=INITIAL_CAP; s->items=malloc(sizeof(char*)*s->cap); }
void sl_free(StrList *s){ if(!s) return; for(int i=0;i<s->count;i++) free(s->items[i]); free(s->items); }
void sl_add(StrList *s, const char *str){
    for(int i=0;i<s->count;i++) if(strcmp(s->items[i],str)==0) return;
    if(s->count==s->cap){ s->cap*=2; s->items = realloc(s->items,sizeof(char*)*s->cap); }
    s->items[s->count++] = my_strdup(str);
}
int sl_index(StrList *s, const char *str){ for(int i=0;i<s->count;i++) if(strcmp(s->items[i],str)==0) return i; return -1; }

void pl_init(ProdList *p){ p->count=0; p->cap=INITIAL_CAP; p->items=malloc(sizeof(Production)*p->cap); }
void pl_free(ProdList *p){ if(!p) return; for(int i=0;i<p->count;i++){ for(int j=0;j<p->items[i].rhs_len;j++) free(p->items[i].rhs[j]); free(p->items[i].rhs); } free(p->items); }
void pl_add(ProdList *p, Production prod){ if(p->count==p->cap){ p->cap*=2; p->items=realloc(p->items,sizeof(Production)*p->cap); } p->items[p->count++]=prod; }

int is_epsilon_token(const char *t){ return strcmp(t,"epsilon")==0 || strcmp(t,"ε")==0; }

// FIRST/FOLLOW internal helpers
static int first_contains(FirstTable *ft, int nt, const char *tok){ StrList *s = &ft->sets[nt]; for(int i=0;i<s->count;i++) if(strcmp(s->items[i],tok)==0) return 1; return 0; }
static int first_add(FirstTable *ft, int nt, const char *tok){ StrList *s = &ft->sets[nt]; for(int i=0;i<s->count;i++) if(strcmp(s->items[i],tok)==0) return 0; sl_add(s,tok); return 1; }

static int follow_contains(FollowTable *fot, int nt, const char *tok){ StrList *s = &fot->sets[nt]; for(int i=0;i<s->count;i++) if(strcmp(s->items[i],tok)==0) return 1; return 0; }
static int follow_add(FollowTable *fot, int nt, const char *tok){ StrList *s = &fot->sets[nt]; for(int i=0;i<s->count;i++) if(strcmp(s->items[i],tok)==0) return 0; sl_add(s,tok); return 1; }

// trim
static char *trim(char *s){ while(isspace((unsigned char)*s)) s++; if(*s==0) return s; char *end = s + strlen(s) - 1; while(end>s && isspace((unsigned char)*end)) *end-- = '\0'; return s; }

// split by whitespace into tokens; returns array of strings and sets *len
static char **split_tokens(const char *line, int *len){
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
        *arrow = '\0';
        char *lhs = trim(p);
        char *rhs = trim(arrow+2);
        if(*lhs==0 || *rhs==0) continue;
        sl_add(nonterms, lhs);
        int lhs_idx = sl_index(nonterms, lhs);
        char *rhs_dup = my_strdup(rhs);
        char *token = strtok(rhs_dup, "|");
        while(token){
            char *alttrim = trim(token);
            int cnt = 0;
            char **toks = split_tokens(alttrim, &cnt);
            Production prod;
            prod.lhs = lhs_idx;
            prod.rhs_len = cnt;
            prod.rhs = malloc(sizeof(char*)*cnt);
            for(int i=0;i<cnt;i++){
                prod.rhs[i] = toks[i];
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
    // remove from terminals any symbol that is actually a nonterminal
    for(int i=0;i<nonterms->count;i++){
        for(int j=0;j<terms->count;){
            if(strcmp(terms->items[j], nonterms->items[i])==0){ free(terms->items[j]); terms->items[j] = terms->items[--terms->count]; } else j++;
        }
    }
}

// compute FIRST sets
void compute_first(StrList *nonterms, StrList *terms, ProdList *prods, FirstTable *ft){
    ft->sets = malloc(sizeof(StrList)*nonterms->count);
    for(int i=0;i<nonterms->count;i++) sl_init(&ft->sets[i]);

    int changed = 1;
    while(changed){
        changed = 0;
        for(int pi=0; pi<prods->count; pi++){
            Production *pr = &prods->items[pi];
            int A = pr->lhs;
            if(pr->rhs_len==0){ if(!first_contains(ft,A,"epsilon")){ first_add(ft,A,"epsilon"); changed=1; } continue; }
            if(pr->rhs_len==1 && is_epsilon_token(pr->rhs[0])){ if(!first_contains(ft,A,"epsilon")){ first_add(ft,A,"epsilon"); changed=1; } continue; }
            int all_nullable = 1;
            for(int k=0;k<pr->rhs_len;k++){
                char *X = pr->rhs[k];
                if(sl_index(nonterms, X)==-1){ if(!first_contains(ft,A,X)){ first_add(ft,A,X); changed=1; } all_nullable = 0; break; }
                else {
                    int Xidx = sl_index(nonterms, X);
                    StrList *sx = &ft->sets[Xidx];
                    int had_epsilon = 0;
                    for(int t=0;t<sx->count;t++){
                        if(is_epsilon_token(sx->items[t])){ had_epsilon = 1; continue; }
                        if(!first_contains(ft,A,sx->items[t])){ first_add(ft,A,sx->items[t]); changed=1; }
                    }
                    if(!had_epsilon){ all_nullable = 0; break; }
                }
            }
            if(all_nullable){ if(!first_contains(ft,A,"epsilon")){ first_add(ft,A,"epsilon"); changed=1; } }
        }
    }
}

// helper: compute FIRST of a sequence of symbols (for FOLLOW computation)
static int first_of_sequence(char **seq, int seq_len, StrList *nonterms, FirstTable *ft, StrList *result_strlist){
    if(seq_len == 0) return 1;
    int all_nullable = 1;
    for(int i=0; i<seq_len; i++){
        char *X = seq[i];
        if(sl_index(nonterms, X) == -1){ if(!is_epsilon_token(X)) sl_add(result_strlist, X); all_nullable = 0; break; }
        else {
            int Xidx = sl_index(nonterms, X);
            StrList *sx = &ft->sets[Xidx];
            int had_epsilon = 0;
            for(int t=0; t<sx->count; t++){
                if(is_epsilon_token(sx->items[t])){ had_epsilon = 1; continue; }
                sl_add(result_strlist, sx->items[t]);
            }
            if(!had_epsilon){ all_nullable = 0; break; }
        }
    }
    return all_nullable;
}

// compute FOLLOW sets; start symbol assumed to be nonterms->items[0]
void compute_follow(StrList *nonterms, StrList *terms, ProdList *prods, FirstTable *ft, FollowTable *fot){
    fot->sets = malloc(sizeof(StrList)*nonterms->count);
    for(int i=0;i<nonterms->count;i++) sl_init(&fot->sets[i]);
    follow_add(fot, 0, "$" );

    int changed = 1;
    while(changed){
        changed = 0;
        for(int pi=0; pi<prods->count; pi++){
            Production *pr = &prods->items[pi];
            int A = pr->lhs;
            for(int k=0; k<pr->rhs_len; k++){
                char *B = pr->rhs[k];
                int Bidx = sl_index(nonterms, B);
                if(Bidx == -1) continue;
                if(k+1 < pr->rhs_len){
                    StrList first_gamma; sl_init(&first_gamma);
                    int gamma_nullable = first_of_sequence(&pr->rhs[k+1], pr->rhs_len - k - 1, nonterms, ft, &first_gamma);
                    for(int t=0; t<first_gamma.count; t++){
                        if(!follow_contains(fot, Bidx, first_gamma.items[t])){ follow_add(fot, Bidx, first_gamma.items[t]); changed = 1; }
                    }
                    if(gamma_nullable){ StrList *foa = &fot->sets[A]; for(int t=0;t<foa->count;t++){ if(!follow_contains(fot,Bidx, foa->items[t])){ follow_add(fot,Bidx, foa->items[t]); changed = 1; } } }
                    sl_free(&first_gamma);
                } else {
                    StrList *foa = &fot->sets[A]; for(int t=0;t<foa->count;t++){ if(!follow_contains(fot,Bidx, foa->items[t])){ follow_add(fot,Bidx, foa->items[t]); changed = 1; } }
                }
            }
        }
    }
}

// optional printing helpers
void print_first(FirstTable *ft, StrList *nonterms){ printf("FIRST sets:\n"); for(int i=0;i<nonterms->count;i++){ printf("FIRST(%s) = { ", nonterms->items[i]); StrList *s = &ft->sets[i]; for(int j=0;j<s->count;j++){ printf("%s", s->items[j]); if(j+1 < s->count) printf(", "); } printf(" }\n"); } }

void print_follow(FollowTable *fot, StrList *nonterms){ printf("FOLLOW sets:\n"); for(int i=0;i<nonterms->count;i++){ printf("FOLLOW(%s) = { ", nonterms->items[i]); StrList *s = &fot->sets[i]; for(int j=0;j<s->count;j++){ printf("%s", s->items[j]); if(j+1 < s->count) printf(", "); } printf(" }\n"); } }

// optional save helpers
void save_first(const char *path, FirstTable *ft, StrList *nonterms){ FILE *f = fopen(path,"w"); if(!f){ perror("fopen"); return; } for(int i=0;i<nonterms->count;i++){ fprintf(f, "%s ->", nonterms->items[i]); StrList *s = &ft->sets[i]; for(int j=0;j<s->count;j++){ fprintf(f, " %s", s->items[j]); if(j+1 < s->count) fprintf(f, ","); } fprintf(f, "\n"); } fclose(f); }

void save_follow(const char *path, FollowTable *fot, StrList *nonterms){ FILE *f = fopen(path,"w"); if(!f){ perror("fopen"); return; } for(int i=0;i<nonterms->count;i++){ fprintf(f, "%s ->", nonterms->items[i]); StrList *s = &fot->sets[i]; for(int j=0;j<s->count;j++){ fprintf(f, " %s", s->items[j]); if(j+1 < s->count) fprintf(f, ","); } fprintf(f, "\n"); } fclose(f); }