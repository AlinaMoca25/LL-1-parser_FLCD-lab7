// create_pif.c
// Utility to create PIF file from lexemes using Symbol Table

#include "pif_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED_LOC -1

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <output_pif_file> <lexeme1> [lexeme2] ...\n", argv[0]);
        fprintf(stderr, "Example: %s program.pif bind x := 10\n", argv[0]);
        return 1;
    }
    
    const char *output_file = argv[1];
    int lexeme_count = argc - 2;
    const char **lexemes = (const char **)(argv + 2);
    
    // Generate PIF using symbol table
    PIFEntry *pif_entries = NULL;
    int pif_count = 0;
    
    printf("Generating PIF from %d lexemes...\n", lexeme_count);
    int result = generate_pif_from_tokens(lexemes, lexeme_count, &pif_entries, &pif_count, NULL);
    
    if (result < 0 || pif_count == 0) {
        fprintf(stderr, "Error: Failed to generate PIF\n");
        return 1;
    }
    
    printf("Generated %d PIF entries\n", pif_count);
    
    // Write to file
    if (write_pif_to_file(output_file, pif_entries, pif_count) != 0) {
        fprintf(stderr, "Error: Failed to write PIF file\n");
        free_pif_entries(pif_entries, pif_count);
        return 1;
    }
    
    printf("PIF written to %s\n", output_file);
    
    // Print PIF for verification
    printf("\nPIF contents:\n");
    printf("~~~~ Program Internal Form (PIF) ~~~~\n");
    for (int i = 0; i < pif_count; i++) {
        if (pif_entries[i].bucket == UNUSED_LOC) {
            printf("%-16s %d\n", pif_entries[i].lexeme, -1);
        } else {
            printf("%-16s %d,%d\n", pif_entries[i].lexeme, 
                   pif_entries[i].bucket, pif_entries[i].pos);
        }
    }
    printf("~~~~~~~~ End PIF ~~~~~~~~\n");
    
    free_pif_entries(pif_entries, pif_count);
    return 0;
}

