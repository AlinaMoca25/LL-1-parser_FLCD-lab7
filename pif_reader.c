// pif_reader.c
// PIF (Program Internal Form) reader implementation

#include "pif_reader.h"
#include "lexer_pif_export.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int read_pif_from_file(const char *filename, PIFEntry **entries, int *count) {
    FILE *f = fopen(filename, "r");
    if (!f) return -1;
    
    *entries = NULL;
    *count = 0;
    int capacity = 1024;
    *entries = malloc(sizeof(PIFEntry) * capacity);
    
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        // Skip empty lines and header/footer
        if (strstr(line, "~~~~") || strstr(line, "End PIF")) continue;
        
        // Parse line: lexeme bucket,pos or lexeme -1
        char lexeme[256];
        int bucket = -1, pos = -1;
        
        if (sscanf(line, "%255s %d,%d", lexeme, &bucket, &pos) == 3) {
            // Format: lexeme bucket,pos
        } else if (sscanf(line, "%255s %d", lexeme, &bucket) == 2 && bucket == -1) {
            // Format: lexeme -1
            pos = -1;
        } else {
            // Try just lexeme
            if (sscanf(line, "%255s", lexeme) == 1) {
                bucket = -1;
                pos = -1;
            } else {
                continue; // Skip malformed lines
            }
        }
        
        if (*count >= capacity) {
            capacity *= 2;
            *entries = realloc(*entries, sizeof(PIFEntry) * capacity);
        }
        
        strncpy((*entries)[*count].lexeme, lexeme, 255);
        (*entries)[*count].lexeme[255] = '\0';
        (*entries)[*count].bucket = bucket;
        (*entries)[*count].pos = pos;
        (*count)++;
    }
    
    fclose(f);
    return *count;
}

int read_pif_from_string(const char *pif_str, PIFEntry **entries, int *count) {
    *entries = NULL;
    *count = 0;
    int capacity = 1024;
    *entries = malloc(sizeof(PIFEntry) * capacity);
    
    const char *p = pif_str;
    char line[512];
    
    while (*p) {
        // Read a line
        int i = 0;
        while (*p && *p != '\n' && i < sizeof(line) - 1) {
            line[i++] = *p++;
        }
        line[i] = '\0';
        if (*p == '\n') p++;
        
        // Skip empty lines and header/footer
        if (strstr(line, "~~~~") || strstr(line, "End PIF") || strlen(line) == 0) continue;
        
        // Parse line
        char lexeme[256];
        int bucket = -1, pos = -1;
        
        if (sscanf(line, "%255s %d,%d", lexeme, &bucket, &pos) == 3) {
            // Format: lexeme bucket,pos
        } else if (sscanf(line, "%255s %d", lexeme, &bucket) == 2 && bucket == -1) {
            // Format: lexeme -1
            pos = -1;
        } else {
            if (sscanf(line, "%255s", lexeme) == 1) {
                bucket = -1;
                pos = -1;
            } else {
                continue;
            }
        }
        
        if (*count >= capacity) {
            capacity *= 2;
            *entries = realloc(*entries, sizeof(PIFEntry) * capacity);
        }
        
        strncpy((*entries)[*count].lexeme, lexeme, 255);
        (*entries)[*count].lexeme[255] = '\0';
        (*entries)[*count].bucket = bucket;
        (*entries)[*count].pos = pos;
        (*count)++;
    }
    
    return *count;
}

StrList pif_to_token_list(PIFEntry *entries, int count, StrList *terms) {
    StrList tokens;
    sl_init(&tokens);
    
    for (int i = 0; i < count; i++) {
        // Map lexeme to terminal name (e.g., "bind" -> "BIND")
        const char *terminal = lexeme_to_terminal(entries[i].lexeme);
        if (terminal) {
            sl_add(&tokens, terminal);
        } else {
            // If mapping fails, use lexeme as-is (fallback)
            sl_add(&tokens, entries[i].lexeme);
        }
    }
    
    return tokens;
}

void free_pif_entries(PIFEntry *entries, int count) {
    if (entries) {
        free(entries);
    }
}

