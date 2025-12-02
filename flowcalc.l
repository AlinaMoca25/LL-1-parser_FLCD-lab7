%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flowcalc.tab.h"
#include "st.h"
#include "dfa.h"

#define UNUSED_LOC -1
typedef struct { char lexeme[256]; int bucket; int pos; } PIFEntry;
#define PIF_MAX 4096
static PIFEntry PIF[PIF_MAX];
static int PIF_len = 0;

static int lineNumber = 1;
static SymbolTable *ST_PTR = NULL;
static DFA *ID_PTR = NULL;
static DFA *NUM_PTR = NULL;

static void pif_add(const char* lex, int bucket, int pos) {
    if (PIF_len < PIF_MAX) {
        strncpy(PIF[PIF_len].lexeme, lex, 255);
        PIF[PIF_len].lexeme[255] = '\0';
        PIF[PIF_len].bucket = bucket;
        PIF[PIF_len].pos = pos;
        PIF_len++;
    }
}

static void add_to_st_and_pif(const char* yy) {
    if (!ST_PTR) return;
    int idx = st_put(ST_PTR, yy);
    int b=-1,p=-1;
    if (st_get_location_by_index(ST_PTR, idx, &b, &p) != 0) { b = UNUSED_LOC; p = UNUSED_LOC; }
    pif_add(yy, b, p);
}

extern YYSTYPE yylval;
%}

%option noyywrap
%option yylineno

DIGIT           [0-9]
LETTER          [A-Za-z_]
IDENT           {LETTER}({LETTER}|{DIGIT})*
NUMBER          {DIGIT}+(\.{DIGIT}+)?
STRING          \"([^\"\\]|\\.)*\"

%%

bind            { pif_add("bind", UNUSED_LOC, UNUSED_LOC); return BIND; }
set             { pif_add("set", UNUSED_LOC, UNUSED_LOC); return SET; }
def             { pif_add("def", UNUSED_LOC, UNUSED_LOC); return DEF; }
yield           { pif_add("yield", UNUSED_LOC, UNUSED_LOC); return YIELD; }
when            { pif_add("when", UNUSED_LOC, UNUSED_LOC); return WHEN; }
otherwise       { pif_add("otherwise", UNUSED_LOC, UNUSED_LOC); return OTHERWISE; }
each            { pif_add("each", UNUSED_LOC, UNUSED_LOC); return EACH; }
in              { pif_add("in", UNUSED_LOC, UNUSED_LOC); return IN; }
do              { pif_add("do", UNUSED_LOC, UNUSED_LOC); return DO; }
end             { pif_add("end", UNUSED_LOC, UNUSED_LOC); return END; }
and             { pif_add("and", UNUSED_LOC, UNUSED_LOC); return AND; }
or              { pif_add("or", UNUSED_LOC, UNUSED_LOC); return OR; }
not             { pif_add("not", UNUSED_LOC, UNUSED_LOC); return NOT; }
asc             { pif_add("asc", UNUSED_LOC, UNUSED_LOC); return ASC; }
desc            { pif_add("desc", UNUSED_LOC, UNUSED_LOC); return DESC; }

apply           { pif_add("apply", UNUSED_LOC, UNUSED_LOC); return APPLY; }
keep            { pif_add("keep", UNUSED_LOC, UNUSED_LOC); return KEEP; }
order           { pif_add("order", UNUSED_LOC, UNUSED_LOC); return ORDER; }
dedupe          { pif_add("dedupe", UNUSED_LOC, UNUSED_LOC); return DEDUPE; }
take            { pif_add("take", UNUSED_LOC, UNUSED_LOC); return TAKE; }
skip            { pif_add("skip", UNUSED_LOC, UNUSED_LOC); return SKIP; }
concat          { pif_add("concat", UNUSED_LOC, UNUSED_LOC); return CONCAT; }
joinstr         { pif_add("joinstr", UNUSED_LOC, UNUSED_LOC); return JOINSTR; }
total           { pif_add("total", UNUSED_LOC, UNUSED_LOC); return TOTAL; }
count           { pif_add("count", UNUSED_LOC, UNUSED_LOC); return COUNT; }
avg             { pif_add("avg", UNUSED_LOC, UNUSED_LOC); return AVG; }

true            { pif_add("true", UNUSED_LOC, UNUSED_LOC); yylval.bool_val = 1; return BOOL_LIT; }
false           { pif_add("false", UNUSED_LOC, UNUSED_LOC); yylval.bool_val = 0; return BOOL_LIT; }
none            { pif_add("none", UNUSED_LOC, UNUSED_LOC); return NONE; }

":="            { pif_add(":=", UNUSED_LOC, UNUSED_LOC); return ASSIGN; }
"->"            { pif_add("->", UNUSED_LOC, UNUSED_LOC); return LAMBDA; }
"|>"            { pif_add("|>", UNUSED_LOC, UNUSED_LOC); return PIPELINE; }
"**"            { pif_add("**", UNUSED_LOC, UNUSED_LOC); return POW; }
">="            { pif_add(">=", UNUSED_LOC, UNUSED_LOC); return GE; }
"<="            { pif_add("<=", UNUSED_LOC, UNUSED_LOC); return LE; }
"=="            { pif_add("==", UNUSED_LOC, UNUSED_LOC); return EQ; }
"!="            { pif_add("!=", UNUSED_LOC, UNUSED_LOC); return NE; }
"..<"           { pif_add("..<", UNUSED_LOC, UNUSED_LOC); return RANGE_DOT_LT; }
".."            { pif_add("..", UNUSED_LOC, UNUSED_LOC); return RANGE_DOT; }

"+"             { pif_add("+", UNUSED_LOC, UNUSED_LOC); return PLUS; }
"-"             { pif_add("-", UNUSED_LOC, UNUSED_LOC); return MINUS; }
"*"             { pif_add("*", UNUSED_LOC, UNUSED_LOC); return MUL; }
"/"             { pif_add("/", UNUSED_LOC, UNUSED_LOC); return DIV; }
"%"             { pif_add("%", UNUSED_LOC, UNUSED_LOC); return MOD; }
"<"             { pif_add("<", UNUSED_LOC, UNUSED_LOC); return LT; }
">"             { pif_add(">", UNUSED_LOC, UNUSED_LOC); return GT; }
"="             { pif_add("=", UNUSED_LOC, UNUSED_LOC); return UPDATE; }
"("             { pif_add("(", UNUSED_LOC, UNUSED_LOC); return LPAREN; }
")"             { pif_add(")", UNUSED_LOC, UNUSED_LOC); return RPAREN; }
"["             { pif_add("[", UNUSED_LOC, UNUSED_LOC); return LBRACKET; }
"]"             { pif_add("]", UNUSED_LOC, UNUSED_LOC); return RBRACKET; }
","             { pif_add(",", UNUSED_LOC, UNUSED_LOC); return COMMA; }

{IDENT}         { 
                    if (ID_PTR && NUM_PTR) {
                        int Li = dfa_longest(ID_PTR, yytext);
                        int Ln = dfa_longest(NUM_PTR, yytext);
                        int len = strlen(yytext);
                        if (Li == len && Li >= Ln) {
                            add_to_st_and_pif(yytext);
                            yylval.string_val = strdup(yytext);
                            return IDENTIFIER;
                        } else if (Ln == len) {
                            add_to_st_and_pif(yytext);
                            yylval.string_val = strdup(yytext);
                            return NUMBER;
                        }
                    }
                    
                    if ((yytext[0] >= 'a' && yytext[0] <= 'z') || 
                        (yytext[0] >= 'A' && yytext[0] <= 'Z') || 
                        yytext[0] == '_') {
                        add_to_st_and_pif(yytext);
                        yylval.string_val = strdup(yytext);
                        return IDENTIFIER;
                    }
                    fprintf(stderr, "Lexical error at line %d: illegal token '%s'\n", lineNumber, yytext);
                    return 0;
                }
{NUMBER}        { 
                    if (NUM_PTR) {
                        int Ln = dfa_longest(NUM_PTR, yytext);
                        int len = strlen(yytext);
                        if (Ln == len) {
                            add_to_st_and_pif(yytext);
                            yylval.string_val = strdup(yytext);
                            return NUMBER;
                        }
                    }
                    
                    int valid = 1;
                    for (int i = 0; yytext[i]; i++) {
                        if (!((yytext[i] >= '0' && yytext[i] <= '9') || yytext[i] == '.')) {
                            valid = 0;
                            break;
                        }
                    }
                    if (valid) {
                        add_to_st_and_pif(yytext);
                        yylval.string_val = strdup(yytext);
                        return NUMBER;
                    }
                    fprintf(stderr, "Lexical error at line %d: illegal number '%s'\n", lineNumber, yytext);
                    return 0;
                }
{STRING}        { 
                    add_to_st_and_pif(yytext);
                    yylval.string_val = strdup(yytext);
                    return STRING;
                }

"#".*           { /* skip comment */ }
\r?\n           { lineNumber++; return NL; }
[ \t\r]+        { /* skip whitespace */ }

.               { fprintf(stderr, "Lexical error at line %d: illegal token '%s'\n", lineNumber, yytext); return 0; }

%%

void init_lexer(SymbolTable *st, DFA *id_dfa, DFA *num_dfa) {
    ST_PTR = st;
    ID_PTR = id_dfa;
    NUM_PTR = num_dfa;
    PIF_len = 0;
    lineNumber = 1;
}

void dump_pif(void) {
    printf("~~~~ Program Internal Form (PIF) ~~~~\n");
    for (int i=0;i<PIF_len;i++) {
        if (PIF[i].bucket == UNUSED_LOC) {
            printf("%-16s %d\n", PIF[i].lexeme, -1);
        } else {
            printf("%-16s %d,%d\n", PIF[i].lexeme, PIF[i].bucket, PIF[i].pos);
        }
    }
    printf("~~~~~~~~ End PIF ~~~~~~~~\n\n");
}

