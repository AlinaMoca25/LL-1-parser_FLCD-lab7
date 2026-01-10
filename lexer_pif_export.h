// lexer_pif_export.h
// Export PIF data from lexer for tree-building parser

#ifndef LEXER_PIF_EXPORT_H
#define LEXER_PIF_EXPORT_H

#include "pif_reader.h"  // Use PIFEntry from here

// Map lexeme to terminal name (for grammar terminals)
// Returns terminal name or NULL if not found
const char *lexeme_to_terminal(const char *lexeme);

#endif // LEXER_PIF_EXPORT_H

