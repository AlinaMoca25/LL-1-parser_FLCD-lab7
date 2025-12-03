LL(1) Parser Generator & Analysis Tools

Complete implementation for LL(1) grammar analysis and parsing: FIRST/FOLLOW computation, LL(1) parse table construction, and parser simulation with multiple output formats.

Project layout
**Core modules:**
- `first_follow.c` / `first_follow.h` : grammar loader, FIRST and FOLLOW set computation.
- `parse_table.c` / `parse_table.h` : LL(1) parse table builder and printer.
- `main_parse_table.c` : driver to load grammar, compute FIRST/FOLLOW, build and display the parse table.
- `LL1.c` : standalone tool for FIRST/FOLLOW computation with debug and save options (legacy, still useful for inspection).

**Grammar files:**
- `grammar.ll1` : example grammar (S -> B b | C d; B -> a B | epsilon; C -> c C | epsilon).
- `grammar_test.ll1` : test grammar for verification.

**Generated outputs (ignored by `.gitignore`):**
- `LL1.exe` / `parse_table.exe` : compiled binaries.
- `first_table.txt` / `follow_table.txt` : saved FIRST/FOLLOW sets (optional).

Prerequisites
- `gcc` (or any C compiler supporting C11)
- `git` for repository operations

Build & Run

**Option 1: Standalone FIRST/FOLLOW tool (LL1.c)**
```powershell
gcc -std=c11 -O2 -o LL1.exe LL1.c
.\LL1.exe grammar.ll1
.\LL1.exe grammar.ll1 --debug
.\LL1.exe grammar.ll1 --save
```

**Option 2: LL(1) Parse Table builder (requires first_follow.c)**
```powershell
gcc -std=c11 -O2 -o parse_table.exe main_parse_table.c parse_table.c first_follow.c
.\parse_table.exe grammar.ll1
```

**Option 3: Full parser with multiple output formats (when implemented)**
- Once your colleague completes the parser engine, compile with:
```powershell
gcc -std=c11 -O2 -o parser.exe main_parser.c parser.c parse_table.c first_follow.c
.\parser.exe grammar.ll1 input_sequence.txt [output_format]
```
Output formats: `productions` (2.a), `derivations` (2.b), or `tree` (2.c).

## Grammar file format
- Lines starting with `#` are comments and ignored.
- Productions are written like `S -> A b | c` (use `|` for alternatives).
- Use token `epsilon` (or `ε`) to denote the empty production.

Example (from repository)
```
S -> B b
S -> C d

B -> a B
B -> epsilon

C -> c C
C -> epsilon
```

## Workflow for Requirements

**Requirement 1**: LL(1) parser implementation and test
- Input: grammar file + token sequence
- Output: string of productions (or accept/reject)
- Status: Infrastructure complete (FIRST/FOLLOW/parse table ready). Awaiting parser engine implementation.

**Requirement 2**: Parse tree representations (multiple output formats)
- Input: grammar + PIF (Program Internal Form / token sequence)
- Output: one of three formats (increasing complexity/grade):
  - 2.a. Productions list
  - 2.b. Derivations sequence
  - 2.c. Parse tree table (father-sibling representation)
- Status: Parse table ready. Implement parser engine to output chosen format.

**Recommended approach**:
1. Implement parser engine with output format 2.a (easiest, unblocks testing).
2. Extend to 2.b (derivations — slight modification).
3. Enhance to 2.c (parse tree — requires tree node construction).

## Testing

Test grammars provided:
- `grammar.ll1` : simple grammar (S, B, C nonterminals).
- `grammar_test.ll1` : more complex grammar (S, A, B, C, D nonterminals).

Run FIRST/FOLLOW on test grammars:
```powershell
.\LL1.exe grammar_test.ll1 --debug
.\parse_table.exe grammar_test.ll1
```

Check `.gitignore` for ignored build artifacts.

## Notes
- All modules use portable C11 (tested on Windows PowerShell with gcc).
- FIRST/FOLLOW computation is complete and tested.
- Parse table construction is ready for use by the parser engine.
- See individual branches (`main`, `construct-parse-table`, `father-sibling-representation`) for implementation progress.
