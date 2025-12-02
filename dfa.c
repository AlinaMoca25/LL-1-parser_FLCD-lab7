#include "dfa.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void dfa_init(DFA* d){
    d->nstates = 0; d->start = 0;
    memset(d->finals, 0, sizeof(d->finals));
    for (int i=0;i<DFA_MAX_STATES;i++)
        for (int c=0;c<DFA_ALPHA;c++) d->trans[i][c] = -1;
}

static int parse_ints(const char* s, int out[], int max){
    int n=0; const char* p=s;
    while (*p && n<max){
        while (*p && (isspace((unsigned char)*p) || *p==',')) ++p;
        if (!*p) break;
        int v=0; while (*p && isdigit((unsigned char)*p)) { v = 10*v + (*p-'0'); ++p; }
        out[n++] = v;
        while (*p && (isspace((unsigned char)*p) || *p==',')) ++p;
    }
    return n;
}

int dfa_load(const char* path, DFA* d){
    dfa_init(d);
    FILE* f = fopen(path, "r"); if (!f) return -1;
    char line[256];
    int seen_start=0, seen_finals=0;

    while (fgets(line, sizeof(line), f)) {
        if (line[0]=='#' || isspace((unsigned char)line[0])) continue;

        if (strncmp(line,"start:",6)==0) {
            int v=0; sscanf(line+6,"%d",&v);
            d->start = v-1; if (d->start<0) d->start=0; seen_start=1;
            if (d->nstates < v) d->nstates = v;
        }
        else if (strncmp(line,"finals:",7)==0) {
            int tmp[128]; int n = parse_ints(line+7, tmp, 128);
            for (int i=0;i<n;i++) if (tmp[i]>0 && tmp[i]<=DFA_MAX_STATES){
                d->finals[tmp[i]-1] = 1;
                if (d->nstates < tmp[i]) d->nstates = tmp[i];
            }
            seen_finals=1;
        }
        else if (strncmp(line,"transitions:",12)==0) {
        }
        else {
            int from=0, to=0; char sym=0;
            if (sscanf(line,"%d %c %d",&from,&sym,&to)==3) {
                if (from>0 && from<=DFA_MAX_STATES && to>0 && to<=DFA_MAX_STATES) {
                    d->trans[from-1][(unsigned char)sym] = to-1;
                    if (d->nstates < from) d->nstates = from;
                    if (d->nstates < to)   d->nstates = to;
                }
            }
        }
    }
    fclose(f);
    if (!seen_start || !seen_finals) return -2;
    return 0;
}

int dfa_longest(const DFA* d, const char* s){
    int state = d->start;
    int i=0, last_accept=-1;
    while (s[i]) {
        int c = (unsigned char)s[i];
        if (c>=DFA_ALPHA) break;
        int nx = d->trans[state][c];
        if (nx == -1) break;
        state = nx;
        ++i;
        if (d->finals[state]) last_accept = i;
    }
    return (last_accept<0) ? 0 :last_accept;
}
