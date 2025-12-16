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
    
    // If lexeme looks like a canonical token name (all uppercase letters/underscores), don't add to ST
    int all_upper = 1;
    for (int i = 0; lexeme[i]; i++) {
        if (!(isupper((unsigned char)lexeme[i]) || lexeme[i] == '_')) { all_upper = 0; break; }
    }
    if (all_upper) return 0;

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

    // Improved tokenizer: splits identifiers, numbers, strings, and operators
    const char *tokens[4096];
    int token_count = 0;

    const char *p = input;

    while (*p && token_count < 4096) {
        // Whitespace handling: treat newlines as explicit NL tokens (grammar expects NL between statements)
        if (isspace((unsigned char)*p)) {
            if (*p == '\n') { tokens[token_count++] = strdup("NL"); p++; continue; }
            if (*p == '\r') { if (p[1] == '\n') { tokens[token_count++] = strdup("NL"); p += 2; continue; } p++; continue; }
            p++; continue;
        }

        // Identifiers / keywords (letters or underscore)
        if (isalpha((unsigned char)*p) || *p == '_') {
            char buf[256]; int n = 0;
            while ((*p) && (isalnum((unsigned char)*p) || *p == '_') && n < (int)sizeof(buf)-1) {
                buf[n++] = *p++; }
            buf[n] = '\0'; tokens[token_count++] = strdup(buf); continue;
        }

        // Numbers: digits with optional single dot (decimal). Stop at '..' sequence (range)
        if (isdigit((unsigned char)*p)) {
            char buf[256]; int n = 0; int has_dot = 0;
            while (*p && n < (int)sizeof(buf)-1) {
                if (*p == '.') {
                    if (p[1] == '.') break; // range operator starts here
                    if (has_dot) break; // second dot -> stop
                    has_dot = 1; buf[n++] = *p++; continue;
                }
                if (!isdigit((unsigned char)*p)) break;
                buf[n++] = *p++;
            }
            buf[n] = '\0'; tokens[token_count++] = strdup(buf); continue;
        }

        // Strings quoted with double quotes
        if (*p == '"') {
            char buf[512]; int n = 0; buf[n++] = *p++;
            while (*p && n < (int)sizeof(buf)-1) {
                buf[n++] = *p; if (*p == '"') { p++; break; } p++; }
            buf[n] = '\0'; tokens[token_count++] = strdup(buf); continue;
        }

        // Multi-char operators
        if (p[0] == '.' && p[1] == '.' && p[2] == '<') { tokens[token_count++] = strdup("..<"); p += 3; continue; }
        if (p[0] == ':' && p[1] == '=') { tokens[token_count++] = strdup(":="); p += 2; continue; }
        if (p[0] == '-' && p[1] == '>') { tokens[token_count++] = strdup("->"); p += 2; continue; }
        if (p[0] == '|' && p[1] == '>') { tokens[token_count++] = strdup("|>"); p += 2; continue; }
        if (p[0] == '*' && p[1] == '*') { tokens[token_count++] = strdup("**"); p += 2; continue; }
        if (p[0] == '>' && p[1] == '=') { tokens[token_count++] = strdup(">="); p += 2; continue; }
        if (p[0] == '<' && p[1] == '=') { tokens[token_count++] = strdup("<="); p += 2; continue; }
        if (p[0] == '=' && p[1] == '=') { tokens[token_count++] = strdup("=="); p += 2; continue; }
        if (p[0] == '!' && p[1] == '=') { tokens[token_count++] = strdup("!="); p += 2; continue; }
        if (p[0] == '.' && p[1] == '.') { tokens[token_count++] = strdup(".."); p += 2; continue; }

        // Single-char tokens / operators
        char single[2] = { *p, '\0' };
        // Accept parentheses, brackets, commas, plus/minus etc.
        if (strchr("()[],+-*/%<>:=|", *p)) {
            tokens[token_count++] = strdup(single);
            p++; continue;
        }

        // Unknown char: treat it as one-char token and continue
        tokens[token_count++] = strdup(single);
        p++;
    }

    // Post-process tokens: merge NUMBER '..' NUMBER into a single RANGE token (e.g., "1..20")
    const char *merged[4096]; int mcount = 0;
    for (int i = 0; i < token_count; ) {
        if (i + 2 < token_count && tokens[i] && tokens[i+1] && tokens[i+2]) {
            // check if pattern number .. number
            int is_num1 = 1, is_num2 = 1;
            for (int k = 0; tokens[i][k]; k++) if (!isdigit((unsigned char)tokens[i][k]) && tokens[i][k] != '.') { is_num1 = 0; break; }
            if (strcmp(tokens[i+1], "..") == 0) {
                for (int k = 0; tokens[i+2][k]; k++) if (!isdigit((unsigned char)tokens[i+2][k]) && tokens[i+2][k] != '.') { is_num2 = 0; break; }
                if (is_num1 && is_num2) {
                    char *r = malloc(strlen(tokens[i]) + strlen(tokens[i+2]) + 3);
                    sprintf(r, "%s..%s", tokens[i], tokens[i+2]);
                    merged[mcount++] = r;
                    i += 3; continue;
                }
            }
        }
        merged[mcount++] = strdup(tokens[i]);
        i++;
    }

    int result = generate_pif_from_tokens(merged, mcount, pif_entries, pif_count, st);

    // Free duplicated tokens
    for (int i = 0; i < token_count; i++) free((void*)tokens[i]);
    for (int i = 0; i < mcount; i++) free((void*)merged[i]);

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

