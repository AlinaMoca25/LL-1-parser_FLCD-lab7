#ifndef DFA_H
#define DFA_H

#define DFA_MAX_STATES 64
#define DFA_ALPHA 128

typedef struct {
    int nstates;
    int start;
    unsigned char finals[DFA_MAX_STATES];
    int trans[DFA_MAX_STATES][DFA_ALPHA];
} DFA;

int  dfa_load(const char* path, DFA* d);
int  dfa_longest(const DFA* d, const char* s);

#endif
