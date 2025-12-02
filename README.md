LL(1) FIRST-set helper

Small helper program and examples used for LL(1) parser lab.

Project layout
- `LL1.c` : program that loads a simple grammar file and computes FIRST sets.
- `grammar.ll1` : example grammar used earlier.
- `grammar_test.ll1` : test grammar you provided.
- `LL1.exe` : compiled binary (ignored by `.gitignore`).

Prerequisites
- `gcc` (or any C compiler supporting C11)
- `git` for repository operations

Build
```powershell
gcc -std=c11 -O2 -o LL1.exe LL1.c
```

Run
```powershell
# compute FIRST on default grammar.ll1
.\LL1.exe grammar.ll1

# compute FIRST on a specific grammar file
.\LL1.exe grammar_test.ll1

# save FIRST table to first_table.txt
.\LL1.exe grammar_test.ll1 --save

# debug: show parsed nonterminals, terminals and productions
.\LL1.exe grammar_test.ll1 --debug
```

Grammar file format
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

