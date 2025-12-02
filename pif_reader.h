// pif_reader.h
// PIF (Program Internal Form) reader for Requirement 2

#ifndef PIF_READER_H
#define PIF_READER_H

#include "first_follow.h"

// PIF entry structure
typedef struct {
    char lexeme[256];
    int bucket;
    int pos;
} PIFEntry;

// Read PIF from file (format: lexeme bucket,pos or lexeme -1)
// Returns number of entries read, or -1 on error
int read_pif_from_file(const char *filename, PIFEntry **entries, int *count);

// Read PIF from string (same format)
int read_pif_from_string(const char *pif_str, PIFEntry **entries, int *count);

// Convert PIF entries to token list for parser
StrList pif_to_token_list(PIFEntry *entries, int count, StrList *terms);

// Free PIF entries
void free_pif_entries(PIFEntry *entries, int count);

#endif // PIF_READER_H

