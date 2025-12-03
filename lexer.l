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


void saveProgramInternalForm(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        return;
    }
    
    for (int i = 0; i < pifLength; i++) {
        fprintf(fp, "Token: %-15s ", ProgramInternalForm[i].token);
        
        if (ProgramInternalForm[i].bucket == -1)
            fprintf(fp, "ST Pos: (-1, -1)\n");
        else
            fprintf(fp, "ST Pos: (%d, %d)\n",
                    ProgramInternalForm[i].bucket,
                    ProgramInternalForm[i].offset);
    }
    
    fclose(fp);
    printf("PIF saved to %s\n", filename);
}

%}

/*** Macros ***/
STRING   "\""([^"\\\n\r]|(\\.))*"\""
ID       [A-Za-z_][A-Za-z0-9_]*
NUMBER   [0-9]+

/*** Rules Section ***/
%%
begin            { addToPIF("BEGIN_PROG", nullPos); }
\{               { addToPIF("{", nullPos); }
\}               { addToPIF("}", nullPos); }
\;               { addToPIF(";", nullPos); }
\:               { addToPIF(":", nullPos); }
\,               { addToPIF(",", nullPos); }
\->              { addToPIF("ARROW", nullPos); }
input            { addToPIF("INPUT", nullPos); }
split            { addToPIF("SPLIT", nullPos); }
by               { addToPIF("BY", nullPos); }
join             { addToPIF("JOIN", nullPos); }
with             { addToPIF("WITH", nullPos); }
replace          { addToPIF("REPLACE", nullPos); }
in               { addToPIF("IN", nullPos); }
rotate           { addToPIF("ROTATE", nullPos); }
positions        { addToPIF("POSITIONS", nullPos); }
send             { addToPIF("SEND", nullPos); }
\+               { addToPIF("+", nullPos); }
\-               { addToPIF("-", nullPos); }
insert           { addToPIF("INSERT", nullPos); }
into             { addToPIF("INTO", nullPos); }
capitalize       { addToPIF("CAPITALIZE", nullPos); }
upper            { addToPIF("UPPER", nullPos); }
lower            { addToPIF("LOWER", nullPos); }
if               { addToPIF("IF", nullPos); }
contains         { addToPIF("CONTAINS", nullPos); }
starts           { addToPIF("STARTS", nullPos); }
ends             { addToPIF("ENDS", nullPos); }
string           { addToPIF("STRING_TYPE", nullPos); }
list             { addToPIF("LIST_TYPE", nullPos); }
left             { addToPIF("LEFT", nullPos); }
right            { addToPIF("RIGHT", nullPos); }
it               { addToPIF("IT", nullPos); }
".each"          { addToPIF("EACH", nullPos); }
\(               { addToPIF("(", nullPos); }
\)               { addToPIF(")", nullPos); }

\n               { lineNumber++; }
[ \t\r]+         { /* skip whitespace */ }

{ID}             { 
                     STPosition pos = addToST(yytext); 
                     addToPIF("IDENTIFIER_TOKEN", pos); /* FIXED NAME */
                 }
{STRING}         { 
                     STPosition pos = addToST(yytext); 
                     addToPIF("STRING_LITERAL_TOKEN", pos); /* FIXED NAME */
                 }
{NUMBER}         { 
                     STPosition pos = addToST(yytext); 
                     addToPIF("NUMBER_TOKEN", pos); /* FIXED NAME */
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
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file> [pif_output_file]\n", argv[0]);
        fprintf(stderr, "  Default PIF output file: pif_output.txt\n");
        return 1;
    }

    const char *inputFile = argv[1];
    const char *pifFile = argc >= 3 ? argv[2] : "pif_output.txt";

    yyin = fopen(inputFile, "r");
    if (!yyin) {
        perror("fopen input file");
        return 1;
    }

    init();
    yylex();
    fclose(yyin);

    // Print to stdout
    showSymbolTable();
    showProgramInternalForm();

    // Save to file
    saveProgramInternalForm(pifFile);

    return errorFound ? 1 : 0;
}