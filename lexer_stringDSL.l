/*** Definition Section ***/
%option prefix="scan"
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

extern FILE *yyin;

typedef struct {
    char token[257];
    int bucket;     // hash index
    int offset;     // position in chain
} PIF;

SymbolTable symbol_table;

PIF ProgramInternalForm[300];
int pifLength = 0;

int errorFound = 0;
int lineNumber = 1;

int symbolIdCounter = 0;
STPosition nullPos = { -1, -1 };

void init() {
    initSymbolTable(&symbol_table);
}

STPosition addToST(char *token) {
    unsigned int bucket = hash(token);
    Symbol *node = symbol_table.table[bucket];
    int offset = 0;

    // search for existing symbol
    while (node) {
        if (strcmp(node->name, token) == 0) {
            STPosition pos = { bucket, offset };
            return pos;
        }
        node = node->next;
        offset++;
    }

    // not found -> insert new
    insert(&symbol_table, token, "identifier", 0);

    STPosition pos = { bucket, 0 }; // new element is at this position
    return pos;
}


void addToPIF(char *token, STPosition pos) {
    strcpy(ProgramInternalForm[pifLength].token, token);
    ProgramInternalForm[pifLength].bucket = pos.bucket;
    ProgramInternalForm[pifLength].offset = pos.offset;
    pifLength++;
}


void showSymbolTable() {
    printf("~~~~~~~ Symbol Table ~~~~~~~\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol* node = symbol_table.table[i];
        int offset = 0;

        while (node) {
            printf("(%d, %d)  %s\n", i, offset, node->name);
            node = node->next;
            offset++;
        }
    }

    printf("~~~~~~~ End Symbol Table ~~~~~~~\n\n");
}


void showProgramInternalForm() {
    printf("~~~~~~~ Program Internal Form ~~~~~~~\n");

    for (int i = 0; i < pifLength; i++) {
        printf("Token: %-15s ", ProgramInternalForm[i].token);

        if (ProgramInternalForm[i].bucket == -1)
            printf("ST Pos: (-1, -1)\n");
        else
            printf("ST Pos: (%d, %d)\n",
                   ProgramInternalForm[i].bucket,
                   ProgramInternalForm[i].offset);
    }

    printf("~~~~~~~ End PIF ~~~~~~~\n\n");
}

%}

/*** Macros ***/
STRING   "\""([^"\\\n\r]|(\\.))*"\""
ID       [A-Za-z_][A-Za-z0-9_]*
NUMBER   [0-9]+

/*** Rules Section ***/
%%
begin            { addToPIF(yytext, nullPos); }
\{               { addToPIF(yytext, nullPos); }
\}               { addToPIF(yytext, nullPos); }
\;               { addToPIF(yytext, nullPos); }
\:               { addToPIF(yytext, nullPos); }
\,               { addToPIF(yytext, nullPos); }
\->              { addToPIF(yytext, nullPos); }
input            { addToPIF(yytext, nullPos); }
split            { addToPIF(yytext, nullPos); }
by               { addToPIF(yytext, nullPos); }
join             { addToPIF(yytext, nullPos); }
with             { addToPIF(yytext, nullPos); }
replace          { addToPIF(yytext, nullPos); }
in               { addToPIF(yytext, nullPos); }
rotate           { addToPIF(yytext, nullPos); }
positions        { addToPIF(yytext, nullPos); }
send             { addToPIF(yytext, nullPos); }
\+               { addToPIF(yytext, nullPos); }
\-               { addToPIF(yytext, nullPos); }
insert           { addToPIF(yytext, nullPos); }
into             { addToPIF(yytext, nullPos); }
capitalize       { addToPIF(yytext, nullPos); }
upper            { addToPIF(yytext, nullPos); }
lower            { addToPIF(yytext, nullPos); }
if               { addToPIF(yytext, nullPos); }
contains         { addToPIF(yytext, nullPos); }
starts           { addToPIF(yytext, nullPos); }
ends             { addToPIF(yytext, nullPos); }
string           { addToPIF(yytext, nullPos); }
list             { addToPIF(yytext, nullPos); }
left             { addToPIF(yytext, nullPos); }
right            { addToPIF(yytext, nullPos); }
it               { addToPIF(yytext, nullPos); }
".each"          { addToPIF(yytext, nullPos); }
\(               { addToPIF(yytext, nullPos); }
\)               { addToPIF(yytext, nullPos); }

\n               { lineNumber++; }
[ \t\r]+         { /* skip whitespace */ }

{ID}             { 
                     STPosition pos = addToST(yytext); 
                     addToPIF("IDENTIFIER", pos);
                 }
{STRING}         { 
                     STPosition pos = addToST(yytext); 
                     addToPIF("STRING", pos);
                 }
{NUMBER}         { 
                     STPosition pos = addToST(yytext); 
                     addToPIF("NUMBER", pos);
                 }

. {
    errorFound = 1;
    printf("Illegal token %s at line %d!\n", yytext, lineNumber);
}
%%

/*** User Code Section ***/
int yywrap(void) {
    return 1;
}

int main(int argc, char **argv) {
    init();
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file> [output_pif_file]\n", argv[0]);
        return 1;
    }
    
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Cannot open input file: %s\n", argv[1]);
        return 1;
    }
    
    yyin = input;
    yylex();
    fclose(input);
    
    showSymbolTable();
    showProgramInternalForm();
    
    if (argc >= 3) {
        FILE *pif_out = fopen(argv[2], "w");
        if (pif_out) {
            fprintf(pif_out, "~~~~~~~ Program Internal Form ~~~~~~~\n");
            for (int i = 0; i < pifLength; i++) {
                fprintf(pif_out, "Token: %-15s ", ProgramInternalForm[i].token);
                if (ProgramInternalForm[i].bucket == -1)
                    fprintf(pif_out, "ST Pos: (-1, -1)\n");
                else
                    fprintf(pif_out, "ST Pos: (%d, %d)\n",
                           ProgramInternalForm[i].bucket,
                           ProgramInternalForm[i].offset);
            }
            fprintf(pif_out, "~~~~~~~ End PIF ~~~~~~~\n");
            fclose(pif_out);
            printf("PIF saved to %s\n", argv[2]);
        }
    }
    
    return errorFound ? 1 : 0;
}

