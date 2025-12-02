// lexer_pif_export.c
// Map lexemes to terminal names for parser

#include "lexer_pif_export.h"
#include <string.h>
#include <ctype.h>

// Case-insensitive string comparison (Windows compatibility)
#ifdef _WIN32
#include <string.h>
#define strcasecmp _stricmp
#else
#include <strings.h>  // For strcasecmp on Unix
#endif

// Map lexeme (lowercase string) to terminal name (uppercase)
const char *lexeme_to_terminal(const char *lexeme) {
    if (!lexeme) return NULL;
    
    // Keywords (exact match, case-insensitive)
    if (strcasecmp(lexeme, "bind") == 0) return "BIND";
    if (strcasecmp(lexeme, "set") == 0) return "SET";
    if (strcasecmp(lexeme, "def") == 0) return "DEF";
    if (strcasecmp(lexeme, "yield") == 0) return "YIELD";
    if (strcasecmp(lexeme, "when") == 0) return "WHEN";
    if (strcasecmp(lexeme, "otherwise") == 0) return "OTHERWISE";
    if (strcasecmp(lexeme, "each") == 0) return "EACH";
    if (strcasecmp(lexeme, "in") == 0) return "IN";
    if (strcasecmp(lexeme, "do") == 0) return "DO";
    if (strcasecmp(lexeme, "end") == 0) return "END";
    if (strcasecmp(lexeme, "and") == 0) return "AND";
    if (strcasecmp(lexeme, "or") == 0) return "OR";
    if (strcasecmp(lexeme, "not") == 0) return "NOT";
    if (strcasecmp(lexeme, "asc") == 0) return "ASC";
    if (strcasecmp(lexeme, "desc") == 0) return "DESC";
    
    // Stage keywords
    if (strcasecmp(lexeme, "apply") == 0) return "APPLY";
    if (strcasecmp(lexeme, "keep") == 0) return "KEEP";
    if (strcasecmp(lexeme, "order") == 0) return "ORDER";
    if (strcasecmp(lexeme, "dedupe") == 0) return "DEDUPE";
    if (strcasecmp(lexeme, "take") == 0) return "TAKE";
    if (strcasecmp(lexeme, "skip") == 0) return "SKIP";
    if (strcasecmp(lexeme, "concat") == 0) return "CONCAT";
    if (strcasecmp(lexeme, "joinstr") == 0) return "JOINSTR";
    if (strcasecmp(lexeme, "total") == 0) return "TOTAL";
    if (strcasecmp(lexeme, "count") == 0) return "COUNT";
    if (strcasecmp(lexeme, "avg") == 0) return "AVG";
    
    // Literals
    if (strcasecmp(lexeme, "true") == 0) return "BOOL_LIT";
    if (strcasecmp(lexeme, "false") == 0) return "BOOL_LIT";
    if (strcasecmp(lexeme, "none") == 0) return "NONE";
    
    // Operators (exact match)
    if (strcmp(lexeme, ":=") == 0) return "ASSIGN";
    if (strcmp(lexeme, "->") == 0) return "LAMBDA";
    if (strcmp(lexeme, "|>") == 0) return "PIPELINE";
    if (strcmp(lexeme, "**") == 0) return "POW";
    if (strcmp(lexeme, ">=") == 0) return "GE";
    if (strcmp(lexeme, "<=") == 0) return "LE";
    if (strcmp(lexeme, "==") == 0) return "EQ";
    if (strcmp(lexeme, "!=") == 0) return "NE";
    if (strcmp(lexeme, "..<") == 0) return "RANGE_DOT_LT";
    if (strcmp(lexeme, "..") == 0) return "RANGE_DOT";
    if (strcmp(lexeme, "+") == 0) return "PLUS";
    if (strcmp(lexeme, "-") == 0) return "MINUS";
    if (strcmp(lexeme, "*") == 0) return "MUL";
    if (strcmp(lexeme, "/") == 0) return "DIV";
    if (strcmp(lexeme, "%") == 0) return "MOD";
    if (strcmp(lexeme, "<") == 0) return "LT";
    if (strcmp(lexeme, ">") == 0) return "GT";
    if (strcmp(lexeme, "=") == 0) return "UPDATE";
    
    // Separators
    if (strcmp(lexeme, "(") == 0) return "LPAREN";
    if (strcmp(lexeme, ")") == 0) return "RPAREN";
    if (strcmp(lexeme, "[") == 0) return "LBRACKET";
    if (strcmp(lexeme, "]") == 0) return "RBRACKET";
    if (strcmp(lexeme, ",") == 0) return "COMMA";
    
    // Newline
    if (strcmp(lexeme, "\n") == 0 || strcmp(lexeme, "\r\n") == 0) return "NL";
    
    // String literals (quoted)
    if (lexeme[0] == '"' && lexeme[strlen(lexeme)-1] == '"') return "STRING";
    
    // Number literals (check if it's a number)
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
    if (is_number && strlen(lexeme) > 0) return "NUMBER";
    
    // Identifiers (everything else that's alphanumeric/underscore)
    int is_identifier = 1;
    for (int i = 0; lexeme[i]; i++) {
        if (!isalnum((unsigned char)lexeme[i]) && lexeme[i] != '_') {
            is_identifier = 0;
            break;
        }
    }
    if (is_identifier && strlen(lexeme) > 0) {
        // Check if it's not a keyword (already checked above)
        return "IDENTIFIER";
    }
    
    // Unknown token
    return NULL;
}

// Note: lexer_get_pif would need to be implemented in the lexer file itself
// since PIF is static. For now, we'll use dump_pif output or add export function to lexer.

