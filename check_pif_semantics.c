#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pif_reader.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pif_file>\n", argv[0]);
        return 2;
    }
    const char *pif_file = argv[1];
    PIFEntry *entries = NULL;
    int count = 0;
    if (read_pif_from_file(pif_file, &entries, &count) < 0) {
        fprintf(stderr, "Failed to read PIF %s\n", pif_file);
        return 2;
    }

    // simple dynamic set of declared identifiers (store lexemes)
    char **decl = NULL;
    int dcap = 32, dcount = 0;
    decl = malloc(sizeof(char*) * dcap);

    int error = 0;
    for (int i = 0; i < count; i++) {
        const char *lex = entries[i].lexeme;
        if (!lex) continue;
        if (strcmp(lex, "BIND") == 0) {
            if (i+1 < count) {
                const char *id = entries[i+1].lexeme;
                if (id) {
                    // Add to declared set
                    if (dcount == dcap) { dcap *= 2; decl = realloc(decl, sizeof(char*)*dcap); }
                    decl[dcount++] = strdup(id);
                }
            }
        }
        if (strcmp(lex, "SET") == 0) {
            if (i+1 < count) {
                const char *id = entries[i+1].lexeme;
                int found = 0;
                for (int j = 0; j < dcount; j++) if (strcmp(decl[j], id) == 0) { found = 1; break; }
                if (!found) {
                    fprintf(stderr, "Semantic error: SET of undeclared identifier '%s' at PIF index %d\n", id ? id : "<missing>", i+1);
                    error = 1; break;
                }
            }
        }
    }

    for (int i = 0; i < dcount; i++) free(decl[i]);
    free(decl);
    free_pif_entries(entries, count);
    return error ? 1 : 0;
}
