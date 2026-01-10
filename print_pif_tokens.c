#include <stdio.h>
#include <stdlib.h>
#include "pif_reader.h"
#include "lexer_pif_export.h"

int main(int argc, char **argv) {
    if (argc != 2) { fprintf(stderr, "Usage: %s <pif_file>\n", argv[0]); return 1; }
    PIFEntry *entries = NULL; int count = 0;
    if (read_pif_from_file(argv[1], &entries, &count) < 0) { fprintf(stderr, "Failed to read PIF\n"); return 1; }
    printf("Read %d entries\n", count);
    for (int i=0;i<count;i++) {
        const char *lex = entries[i].lexeme;
        const char *term = lexeme_to_terminal(lex);
        printf("%3d: lex='%-12s' bucket=%d,%d -> terminal=%s\n", i, lex, entries[i].bucket, entries[i].pos, term ? term : "<none>");
    }
    free_pif_entries(entries, count);
    return 0;
}
