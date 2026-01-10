# LL(1) Parser with Parse Tree Generation

A complete LL(1) parser implementation with parse tree generation for Requirement 2. The parser can work with any LL(1) grammar and generates parse trees with father/sibling relations.

## Project Structure

### Core Parser Components
- `parser.c` / `parser.h` - Basic LL(1) parser (productions only)
- `parser_tree.c` / `parser_tree.h` - Tree-building LL(1) parser
- `parse_tree.c` / `parse_tree.h` - Parse tree structure and printing
- `first_follow.c` / `first_follow.h` - FIRST and FOLLOW set computation
- `parse_table.c` / `parse_table.h` - LL(1) parse table construction

### PIF (Program Internal Form) Handling
- `pif_reader.c` / `pif_reader.h` - Reads PIF files
- `pif_generator.c` / `pif_generator.h` - Generates PIF from tokens using Symbol Table
- `lexer_pif_export.c` / `lexer_pif_export.h` - Maps lexemes to terminal names

### Main Programs
- `main_parser.c` - Basic parser (outputs production sequence)
- `main_tree_parser.c` - Tree-building parser (outputs parse tree table)
- `main_parse_table.c` - Parse table builder and printer
- `create_pif.c` - Utility to create PIF files from command-line tokens

### Grammar Files
- `grammar.txt` - FlowCalculation Mini-DSL grammar (LL(1) format)
- `grammar.ll1` - Example grammar
- `grammar_test.ll1` - Test grammar

### Lexer Files (for reference)
- `flowcalc.l` - Flex lexer definition
- `dfa.c` / `dfa.h` - DFA implementation for identifier/number recognition
- `st.c` / `st.h` - Symbol Table implementation
- `identifier.fa` / `number.fa` - DFA definitions

## Prerequisites

- `gcc` (or any C compiler supporting C11)
- A lexer that outputs PIF format (or use `create_pif` utility)

## Building

### Tree-Building Parser (Requirement 2)
```powershell
gcc -std=c11 -Wall -o tree_parser.exe main_tree_parser.c parser_tree.c parse_tree.c pif_reader.c lexer_pif_export.c first_follow.c parse_table.c
```

### Basic Parser
```powershell
gcc -std=c11 -Wall -o parser.exe main_parser.c parser.c first_follow.c parse_table.c
```

### Parse Table Builder
```powershell
gcc -std=c11 -Wall -o parse_table.exe main_parse_table.c first_follow.c parse_table.c
```

### PIF Generator Utility
```powershell
gcc -std=c11 -Wall -o create_pif.exe create_pif.c pif_generator.c pif_reader.c first_follow.c
```

## Usage

### Tree-Building Parser (Main Program)

```powershell
.\tree_parser.exe <grammar_file> <pif_file> [output_file]
```

**Example:**
```powershell
.\tree_parser.exe grammar.txt program.pif parse_tree.txt
```

**Output:** Parse tree table with father/sibling relations showing:
- Node index
- Symbol name
- Type (NTERM/TERM)
- Production index (for nonterminals)
- Father index
- Sibling index
- Lexeme (for terminals)
- Symbol Table location (bucket,pos for identifiers/numbers/strings)

### Basic Parser

```powershell
.\parser.exe "<input_tokens>" <grammar_file>
```

**Example:**
```powershell
.\parser.exe "BIND IDENTIFIER ASSIGN NUMBER" grammar.txt
```

**Output:** Sequence of production indices (e.g., `p0 p2 p3`)

### Parse Table Builder

```powershell
.\parse_table.exe [grammar_file]
```

**Example:**
```powershell
.\parse_table.exe grammar.txt
```

**Output:** LL(1) parse table printed to console

### Create PIF File

```powershell
.\create_pif.exe <output_pif_file> <lexeme1> [lexeme2] ...
```

**Example:**
```powershell
.\create_pif.exe program.pif bind x := 10
```

This generates a PIF file with correct Symbol Table entries for identifiers, numbers, and strings.

## Grammar File Format

- Lines starting with `#` are comments
- Productions: `A -> X Y | Z` (use `|` for alternatives)
- Use `epsilon` or `ε` for empty productions
- Terminals should be uppercase (e.g., `BIND`, `IDENTIFIER`, `NUMBER`)
- Nonterminals should be lowercase (e.g., `program`, `stmt`, `expr`)

**Example:**
```
# Start symbol: program
program -> stmt program_tail
program_tail -> NL stmt program_tail | epsilon
stmt -> bind_stmt | expr
bind_stmt -> BIND id ASSIGN expr
id -> IDENTIFIER
```

## PIF File Format

The PIF (Program Internal Form) file format:
```
~~~~ Program Internal Form (PIF) ~~~~
lexeme           bucket,pos
lexeme           -1
~~~~~~~~ End PIF ~~~~~~~~
```

- Keywords and operators: `-1` (not in Symbol Table)
- Identifiers, numbers, strings: `bucket,pos` (Symbol Table location)

## Using with a Different DSL

The parser framework is **generic** and works with any LL(1) grammar. To use it with a different DSL:

### 1. Create Your Grammar File
Write your grammar in LL(1) format (see Grammar File Format above).

### 2. Update Lexeme-to-Terminal Mapping
Edit `lexer_pif_export.c` and update the `lexeme_to_terminal()` function to map your DSL's lexemes to terminal names:

```c
const char *lexeme_to_terminal(const char *lexeme) {
    // Your DSL's keywords
    if (strcasecmp(lexeme, "if") == 0) return "IF";
    if (strcasecmp(lexeme, "then") == 0) return "THEN";
    // ... add all your keywords
    
    // Your DSL's operators
    if (strcmp(lexeme, "+") == 0) return "PLUS";
    // ... add all your operators
    
    // Common patterns (usually reusable)
    if (lexeme[0] == '"') return "STRING";
    // Number detection (check if all digits)
    // Identifier detection (check if alphanumeric/underscore)
    
    return NULL; // Unknown token
}
```

### 3. Generate PIF from Your Lexer
Run your lexer to generate a PIF file, or use the `create_pif` utility.

### 4. Run the Parser
```powershell
.\tree_parser.exe your_grammar.ll1 your_program.pif output.txt
```

## Features

- ✅ LL(1) parsing algorithm
- ✅ Parse tree generation with father/sibling relations
- ✅ PIF integration (reads lexer output)
- ✅ Symbol Table location tracking
- ✅ Generic framework (works with any LL(1) grammar)
- ✅ Error detection and reporting
- ✅ Production sequence output

## Requirements

### Requirement 1: LL(1) Parse Table
- Compute FIRST and FOLLOW sets
- Build LL(1) parse table
- Analyze sequences based on moves between configurations
- Output production sequence

**Files:** `main_parse_table.c`, `main_parser.c`

### Requirement 2: Parse Tree Generation
- Read PIF from lexer output
- Build parse tree during parsing
- Output parse tree as table (father/sibling relations)
- Include lexeme and Symbol Table information

**Files:** `main_tree_parser.c`, `parser_tree.c`, `parse_tree.c`

## Example Output

### Parse Tree Table
```
Sequence accepted

Parse Tree (Father/Sibling Relations):
========================================

Index | Symbol | Type | Production | Father | Sibling | Lexeme | ST Location
------|--------|------|------------|--------|---------|--------|------------
    0 | program | NTERM |          0 |     -1 |      -1 | -      | -
    1 | stmt   | NTERM |          3 |      0 |      28 | -      | -
    2 | bind_stmt | NTERM |         10 |      1 |      -1 | -      | -
    3 | BIND   | TERM |         -1 |      2 |       4 | bind   | -
    4 | id     | NTERM |         98 |      2 |       6 | -      | -
    5 | IDENTIFIER | TERM |         -1 |      4 |      -1 | x      | 1,0
    6 | ASSIGN | TERM |         -1 |      2 |       7 | :=     | -
    7 | expr   | NTERM |         25 |      2 |      -1 | -      | -
    ...
```

## Notes

- The parser requires LL(1) grammars (no left recursion, no ambiguity)
- Left recursion must be eliminated before use
- The grammar must be in the specified format
- PIF files must follow the standard format
- Terminal names in grammar must match those returned by `lexeme_to_terminal()`

## License

This project is for educational purposes (FLCD Lab 7).
