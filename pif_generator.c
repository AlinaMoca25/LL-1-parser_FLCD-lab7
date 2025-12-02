// pif_generator.c
// PIF generator using Symbol Table for correct bucket/pos values

#include "pif_reader.h"
#include "st.h"
#include "lexer_pif_export.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define UNUSED_LOC -1

// Check if a lexeme needs to be in symbol table (identifiers, numbers, strings)
static int needs_symbol_table(const char *lexeme) {
    if (!lexeme || strlen(lexeme) == 0) return 0;
    
    // Check if it's a keyword or operator (these don't go in ST)
    const char *terminal = lexeme_to_terminal(lexeme);
    if (terminal && strcmp(terminal, lexeme) != 0) {
        // It's a keyword/operator - check if it maps to IDENTIFIER, NUMBER, or STRING
        if (strcmp(terminal, "IDENTIFIER") == 0 || 
            strcmp(terminal, "NUMBER") == 0 || 
            strcmp(terminal, "STRING") == 0) {
            return 1; // These need ST
        }
        return 0; // Keywords/operators don't need ST
    }
    
    // If lexeme_to_terminal returns NULL or same as lexeme, check if it looks like identifier/number/string
    // Identifiers: start with letter or underscore, contain alphanumeric/underscore
    if (isalpha((unsigned char)lexeme[0]) || lexeme[0] == '_') {
        int is_identifier = 1;
        for (int i = 1; lexeme[i]; i++) {
            if (!isalnum((unsigned char)lexeme[i]) && lexeme[i] != '_') {
                is_identifier = 0;
                break;
            }
        }
        if (is_identifier) return 1;
    }
    
    // Numbers: digits with optional decimal point
    int is_number = 1;
    int has_dot = 0;
    for (int i = 0; lexeme[i]; i++) {
        if (lexeme[i] == '.') {
            if (has_dot) { is_number = 0; break; }
            has_dot = 1;
        } else if (!isdigit((unsigned char)lexeme[i])) {
            is_number = 0;
            break;
        }
    }
    if (is_number && strlen(lexeme) > 0) return 1;
    
    // Strings: quoted
    if (lexeme[0] == '"' && lexeme[strlen(lexeme)-1] == '"') return 1;
    
    return 0;
}

// Generate PIF from lexeme list using Symbol Table
// Returns number of PIF entries generated, or -1 on error
int generate_pif_from_tokens(const char **tokens, int token_count, 
                              PIFEntry **pif_entries, int *pif_count,
                              SymbolTable *st) {
    if (!tokens || !pif_entries || !pif_count) return -1;
    
    *pif_entries = NULL;
    *pif_count = 0;
    int capacity = 1024;
    *pif_entries = malloc(sizeof(PIFEntry) * capacity);
    
    // Initialize symbol table if not provided
    SymbolTable local_st;
    int use_local_st = (st == NULL);
    if (use_local_st) {
        st_init(&local_st, 16);
        st = &local_st;
    }
    
    for (int i = 0; i < token_count; i++) {
        const char *lexeme = tokens[i];
        if (!lexeme) continue;
        
        int bucket = UNUSED_LOC;
        int pos = UNUSED_LOC;
        
        // Check if lexeme needs to be in symbol table
        if (needs_symbol_table(lexeme)) {
            // Add to symbol table and get location (same as lexer's add_to_st_and_pif)
            int idx = st_put(st, lexeme);
            if (st_get_location_by_index(st, idx, &bucket, &pos) != 0) {
                bucket = UNUSED_LOC;
                pos = UNUSED_LOC;
            }
        }
        
        // Add to PIF
        if (*pif_count >= capacity) {
            capacity *= 2;
            *pif_entries = realloc(*pif_entries, sizeof(PIFEntry) * capacity);
        }
        
        strncpy((*pif_entries)[*pif_count].lexeme, lexeme, 255);
        (*pif_entries)[*pif_count].lexeme[255] = '\0';
        (*pif_entries)[*pif_count].bucket = bucket;
        (*pif_entries)[*pif_count].pos = pos;
        (*pif_count)++;
    }
    
    // Cleanup local symbol table if used
    if (use_local_st) {
        st_free(&local_st);
    }
    
    return *pif_count;
}

// Generate PIF from input string (tokenizes and builds symbol table)
int generate_pif_from_string(const char *input, PIFEntry **pif_entries, int *pif_count,
                              SymbolTable *st) {
    if (!input || !pif_entries || !pif_count) return -1;
    
    // Simple tokenizer - split by whitespace
    // In real usage, this would use the lexer
    const char *tokens[1024];
    int token_count = 0;
    
    const char *p = input;
    char token[256];
    int token_len = 0;
    
    while (*p && token_count < 1024) {
        if (isspace((unsigned char)*p)) {
            if (token_len > 0) {
                token[token_len] = '\0';
                tokens[token_count] = strdup(token);
                token_count++;
                token_len = 0;
            }
        } else {
            if (token_len < 255) {
                token[token_len++] = *p;
            }
        }
        p++;
    }
    
    if (token_len > 0) {
        token[token_len] = '\0';
        tokens[token_count] = strdup(token);
        token_count++;
    }
    
    int result = generate_pif_from_tokens(tokens, token_count, pif_entries, pif_count, st);
    
    // Free duplicated tokens
    for (int i = 0; i < token_count; i++) {
        free((void*)tokens[i]);
    }
    
    return result;
}

// Write PIF to file in the format expected by the parser
int write_pif_to_file(const char *filename, PIFEntry *pif_entries, int pif_count) {
    if (!filename || !pif_entries) return -1;
    
    FILE *f = fopen(filename, "w");
    if (!f) return -1;
    
    fprintf(f, "~~~~ Program Internal Form (PIF) ~~~~\n");
    for (int i = 0; i < pif_count; i++) {
        if (pif_entries[i].bucket == UNUSED_LOC) {
            fprintf(f, "%-16s %d\n", pif_entries[i].lexeme, -1);
        } else {
            fprintf(f, "%-16s %d,%d\n", pif_entries[i].lexeme, 
                    pif_entries[i].bucket, pif_entries[i].pos);
        }
    }
    fprintf(f, "~~~~~~~~ End PIF ~~~~~~~~\n");
    
    fclose(f);
    return 0;
}

