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

Linking with your GitHub repo
Use these PowerShell commands in the project root. They initialize a local repo (if you haven't already), commit, set the remote, and push to GitHub.

If you have NOT initialized a git repo locally yet:
```powershell
git init; 
git add .; 
git commit -m "Initial commit"; 
git branch -M main; 
git remote add origin https://github.com/AlinaMoca25/LL-1-parser_FLCD-lab7.git; 
git push -u origin main
```

If you already have a remote named `origin` and want to change it:
```powershell
git remote set-url origin https://github.com/AlinaMoca25/LL-1-parser_FLCD-lab7.git; 
git push -u origin main
```

Notes
- If GitHub repo is empty, `git push -u origin main` will create the remote branch.
- If the remote has commits and you need to pull first, run `git pull --rebase origin main` and resolve conflicts before pushing.
- If you want the FIRST table saved in a different format (JSON, CSV, or as a C header), I can add that.

License & attribution
- This is a small lab helper; add a license file if you want to publish publicly.
