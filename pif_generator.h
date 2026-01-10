// pif_generator.h
// PIF generator using Symbol Table

#ifndef PIF_GENERATOR_H
#define PIF_GENERATOR_H

#include "pif_reader.h"
#include "st.h"

// Generate PIF from token list using Symbol Table
// If st is NULL, creates a local symbol table
int generate_pif_from_tokens(const char **tokens, int token_count, 
                              PIFEntry **pif_entries, int *pif_count,
                              SymbolTable *st);

// Generate PIF from input string (simple tokenizer)
int generate_pif_from_string(const char *input, PIFEntry **pif_entries, int *pif_count,
                              SymbolTable *st);

// Write PIF to file in the format expected by the parser
int write_pif_to_file(const char *filename, PIFEntry *pif_entries, int pif_count);

#endif // PIF_GENERATOR_H

