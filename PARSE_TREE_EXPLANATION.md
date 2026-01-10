# Parse Tree Generation with Father/Sibling Relations - Code Explanation

## Overview

The parse tree is built **during LL(1) parsing** by maintaining a parallel stack of tree nodes alongside the parsing stack. Each node has:
- **Father**: Points to parent node
- **Sibling**: Points to next right sibling
- **Child**: Points to leftmost child

This creates a tree structure where siblings are linked horizontally, and children are linked vertically.

---

## 1. Data Structure: ParseTreeNode

```c
typedef struct ParseTreeNode {
    char *symbol;           // Grammar symbol (terminal or nonterminal)
    int is_terminal;         // 1 if terminal, 0 if nonterminal
    int production_index;    // Production index if nonterminal (or -1 for terminal)
    
    // Tree structure - THE KEY RELATIONS
    struct ParseTreeNode *father;      // Parent node (NULL for root)
    struct ParseTreeNode *sibling;     // Next sibling (right sibling, NULL if last)
    struct ParseTreeNode *child;        // First child (leftmost child, NULL if leaf)
    
    // Token info (for terminals)
    char *lexeme;           // Original lexeme from PIF
    int bucket;             // Symbol table bucket
    int pos;                // Symbol table position
} ParseTreeNode;
```

**Tree Structure Visualization:**
```
        A (father=NULL, sibling=NULL, child=B)
        |
    B---C---D  (B: father=A, sibling=C, child=E)
    |   |   |
    E   F   G  (E: father=B, sibling=NULL, child=NULL)
```

---

## 2. Node Creation

### `tree_node_create()` - Creates a new node

```c
ParseTreeNode *tree_node_create(const char *symbol, int is_terminal) {
    ParseTreeNode *node = malloc(sizeof(ParseTreeNode));
    if (!node) return NULL;
    
    node->symbol = malloc(strlen(symbol) + 1);
    strcpy(node->symbol, symbol);
    node->is_terminal = is_terminal;
    node->production_index = -1;  // Will be set when production is applied
    
    // Initialize tree relations to NULL
    node->father = NULL;
    node->sibling = NULL;
    node->child = NULL;
    
    // Initialize token info
    node->lexeme = NULL;
    node->bucket = -1;
    node->pos = -1;
    
    return node;
}
```

**Key Points:**
- All tree pointers start as `NULL`
- Relations are established when nodes are added to the tree
- `production_index` is set later when a production is applied

---

## 3. Adding Children: Establishing Father/Sibling Relations

### `tree_node_add_child()` - Links a child to its parent

```c
void tree_node_add_child(ParseTreeNode *parent, ParseTreeNode *child) {
    if (!parent || !child) return;
    
    // STEP 1: Set child's father pointer
    child->father = parent;
    
    // STEP 2: Add child to parent's children list
    if (!parent->child) {
        // First child - becomes leftmost child
        parent->child = child;
    } else {
        // Not first child - add as rightmost sibling
        ParseTreeNode *sibling = parent->child;
        // Traverse to the rightmost sibling
        while (sibling->sibling) {
            sibling = sibling->sibling;
        }
        // Link new child as rightmost sibling
        sibling->sibling = child;
    }
}
```

**How it works:**
1. **Sets `child->father = parent`** - Establishes parent-child relation
2. **If parent has no children**: `parent->child = child` (first child)
3. **If parent has children**: Traverse siblings to find the rightmost, then `rightmost->sibling = child`

**Example:**
```
Initial: parent->child = A
         A->sibling = NULL

Add B:   parent->child = A
         A->sibling = B
         B->father = parent

Add C:   parent->child = A
         A->sibling = B
         B->sibling = C
         C->father = parent
```

---

## 4. Tree Building During Parsing

### TreeConfiguration Structure

The parser maintains a **parallel node stack** alongside the parsing stack:

```c
typedef struct {
    StrList alpha;      // input stack (w$)
    StrList beta;       // working stack (S$)
    StrList pi;         // output (productions sequence)
    
    // Tree-building additions
    ParseTreeNode **node_stack;  // Stack of nodes (parallel to beta)
    int node_stack_count;
    int node_stack_cap;
    
    ParseTreeNode *root;     // Root of parse tree
    
    PIFEntry *pif_entries;  // PIF entries for terminal info
    int pif_count;
    int pif_index;           // Current PIF entry index
} TreeConfiguration;
```

**Key Idea:** `node_stack` mirrors `beta` stack - each symbol in `beta` has a corresponding node in `node_stack`.

---

## 5. ActionPush: Building Nonterminal Nodes

When a production `A -> X Y Z` is applied:

```c
static int tree_action_push(TreeConfiguration *config, int **table, 
                             StrList *nonterms, StrList *terms, ProdList *prods) {
    // Get nonterminal A from stack
    const char *A = stack_head(&config->beta);
    const char *u = stack_head(&config->alpha);
    
    // Look up production in parse table
    int table_val = table_lookup(table, nonterms, terms, A, u);
    Production *prod = &prods->items[table_val];
    
    // STEP 1: Pop the nonterminal node from node stack
    ParseTreeNode *A_node = pop_node_head(config);
    A_node->production_index = table_val;  // Record which production was used
    
    // STEP 2: Pop A from beta stack
    pop_head(&config->beta);
    
    // STEP 3: Create child nodes for RHS symbols (in REVERSE order for stack)
    if (prod->rhs_len == 0 || is_epsilon(prod->rhs[0])) {
        // Epsilon production: no children
    } else {
        ParseTreeNode *children[256];
        int child_count = 0;
        
        // Push RHS in reverse order and create nodes
        for (int i = prod->rhs_len - 1; i >= 0; i--) {
            const char *symbol = prod->rhs[i];
            push_head(&config->beta, symbol);  // Push to parsing stack
            
            // Create node for this symbol
            int is_term = (sl_index(terms, symbol) != -1);
            ParseTreeNode *child_node = tree_node_create(symbol, is_term);
            
            // Store in array (will be reversed later)
            children[child_count++] = child_node;
            
            // Push node to node stack (parallel to beta)
            push_node_head(config, child_node);
        }
        
        // STEP 4: Add children to A_node in LEFT-TO-RIGHT order
        // (reverse of what we collected, since we pushed in reverse)
        for (int i = child_count - 1; i >= 0; i--) {
            tree_node_add_child(A_node, children[i]);
        }
    }
    
    return 1;
}
```

**Why reverse order?**
- Stacks are LIFO (Last In, First Out)
- We want children in left-to-right order: `A -> X Y Z`
- So we push in reverse: `Z, Y, X` (Z on top)
- When we pop, we get `X, Y, Z` in correct order

**Example: Production `stmt -> BIND id ASSIGN expr`**

```
Before:
  beta: [stmt, $]
  node_stack: [stmt_node, NULL]

After tree_action_push:
  beta: [expr, ASSIGN, id, BIND, $]  (pushed in reverse)
  node_stack: [expr_node, ASSIGN_node, id_node, BIND_node, stmt_node, NULL]
  
  stmt_node->child = BIND_node
  BIND_node->sibling = id_node
  id_node->sibling = ASSIGN_node
  ASSIGN_node->sibling = expr_node
  
  All children have: father = stmt_node
```

---

## 6. ActionPop: Creating Terminal Nodes with PIF Info

When a terminal is matched:

```c
static int tree_action_pop(TreeConfiguration *config) {
    const char *terminal = stack_head(&config->beta);
    ParseTreeNode *term_node = pop_node_head(config);
    
    // Associate PIF entry with terminal node
    if (strcmp(terminal, "$") != 0 && config->pif_index < config->pif_count) {
        PIFEntry *entry = &config->pif_entries[config->pif_index];
        
        // Copy lexeme and Symbol Table location
        term_node->lexeme = malloc(strlen(entry->lexeme) + 1);
        strcpy(term_node->lexeme, entry->lexeme);
        term_node->bucket = entry->bucket;
        term_node->pos = entry->pos;
        
        config->pif_index++;  // Move to next PIF entry
    }
    
    // Pop from both stacks
    pop_head(&config->alpha);
    pop_head(&config->beta);
    
    return 1;
}
```

**What happens:**
- Terminal node already exists (created during ActionPush)
- We just need to fill in the lexeme and Symbol Table info from PIF
- The father/sibling relations are already established

---

## 7. Printing the Tree: Father/Sibling Relations

### `tree_print_table()` - Outputs tree as formatted table

```c
void tree_print_table(ParseTreeNode *root, FILE *out) {
    // STEP 1: Collect all nodes in pre-order traversal
    ParseTreeNode **nodes = malloc(sizeof(ParseTreeNode*) * 10000);
    int node_count = 0;
    collect_nodes(root, nodes, &node_count, 10000);
    
    // STEP 2: Print header
    fprintf(out, "Index | Symbol | Type | Production | Father | Sibling | Lexeme | ST Location\n");
    
    // STEP 3: For each node, find its father and sibling indices
    for (int i = 0; i < node_count; i++) {
        ParseTreeNode *node = nodes[i];
        
        // Find father index
        int father_idx = -1;
        if (node->father) {
            for (int j = 0; j < node_count; j++) {
                if (nodes[j] == node->father) {
                    father_idx = j;
                    break;
                }
            }
        }
        
        // Find sibling index
        int sibling_idx = -1;
        if (node->sibling) {
            for (int j = 0; j < node_count; j++) {
                if (nodes[j] == node->sibling) {
                    sibling_idx = j;
                    break;
                }
            }
        }
        
        // Print node info
        fprintf(out, "%5d | %-6s | %-4s | %10d | %6d | %7d | %-6s | ",
                i,  // node index
                node->symbol,
                node->is_terminal ? "TERM" : "NTERM",
                node->production_index,
                father_idx,    // -1 if no father (root)
                sibling_idx,  // -1 if no sibling (last child)
                node->lexeme ? node->lexeme : "-");
        
        // Print Symbol Table location
        if (node->bucket >= 0 && node->pos >= 0) {
            fprintf(out, "%d,%d", node->bucket, node->pos);
        } else {
            fprintf(out, "-");
        }
        fprintf(out, "\n");
    }
}
```

**Helper: `collect_nodes()` - Pre-order traversal**

```c
static void collect_nodes(ParseTreeNode *node, ParseTreeNode **nodes, 
                          int *count, int max) {
    if (!node || *count >= max) return;
    
    // Visit node
    nodes[(*count)++] = node;
    
    // Visit children (left to right via sibling chain)
    ParseTreeNode *child = node->child;
    while (child) {
        collect_nodes(child, nodes, count, max);
        child = child->sibling;  // Move to next sibling
    }
}
```

**Output Example:**
```
Index | Symbol | Type | Production | Father | Sibling | Lexeme | ST Location
------|--------|------|------------|--------|---------|--------|------------
    0 | program | NTERM |          0 |     -1 |      -1 | -      | -
    1 | stmt   | NTERM |          3 |      0 |      28 | -      | -
    2 | bind_stmt | NTERM |         10 |      1 |      -1 | -      | -
    3 | BIND   | TERM |         -1 |      2 |       4 | bind   | -
    4 | id     | NTERM |         98 |      2 |       6 | -      | -
    5 | IDENTIFIER | TERM |         -1 |      4 |      -1 | x      | 1,0
    6 | ASSIGN | TERM |         -1 |      2 |       7 | :=     | -
```

**Reading the table:**
- **Index 0** (program): Root node, no father (-1), no sibling (-1)
- **Index 1** (stmt): Father is 0 (program), no sibling (-1, it's the only child)
- **Index 2** (bind_stmt): Father is 1 (stmt), no sibling (-1)
- **Index 3** (BIND): Father is 2 (bind_stmt), sibling is 4 (id)
- **Index 4** (id): Father is 2 (bind_stmt), sibling is 6 (ASSIGN)
- **Index 5** (IDENTIFIER): Father is 4 (id), no sibling (-1), lexeme="x", ST=1,0

---

## 8. Complete Example: Building a Tree

**Input:** `bind x := 10`

**Grammar:** `bind_stmt -> BIND id ASSIGN expr`

**Step-by-step:**

1. **Initialization:**
   ```
   beta: [program, $]
   node_stack: [program_node, NULL]
   ```

2. **Apply `program -> stmt program_tail`:**
   ```
   beta: [program_tail, stmt, $]
   node_stack: [program_tail_node, stmt_node, program_node, NULL]
   
   program_node->child = stmt_node
   stmt_node->father = program_node
   ```

3. **Apply `stmt -> bind_stmt`:**
   ```
   beta: [bind_stmt, program_tail, $]
   node_stack: [bind_stmt_node, stmt_node, program_tail_node, program_node, NULL]
   
   stmt_node->child = bind_stmt_node
   bind_stmt_node->father = stmt_node
   ```

4. **Apply `bind_stmt -> BIND id ASSIGN expr`:**
   ```
   beta: [expr, ASSIGN, id, BIND, program_tail, $]
   node_stack: [expr_node, ASSIGN_node, id_node, BIND_node, bind_stmt_node, ...]
   
   bind_stmt_node->child = BIND_node
   BIND_node->father = bind_stmt_node
   BIND_node->sibling = id_node
   id_node->father = bind_stmt_node
   id_node->sibling = ASSIGN_node
   ASSIGN_node->father = bind_stmt_node
   ASSIGN_node->sibling = expr_node
   expr_node->father = bind_stmt_node
   ```

5. **Pop terminals (BIND, id, ASSIGN, expr):**
   - Fill in lexemes from PIF
   - Set Symbol Table locations
   - Tree structure is already complete!

---

## Key Design Decisions

1. **Parallel Node Stack**: Maintains 1:1 correspondence with parsing stack
2. **Reverse Order Pushing**: Ensures children are in correct left-to-right order
3. **Father/Sibling Links**: Efficient tree navigation without parent pointers in children
4. **PIF Integration**: Terminal nodes get lexeme and Symbol Table info during ActionPop

---

## Summary

The parse tree is built **incrementally during parsing**:
- **ActionPush**: Creates nonterminal nodes and links children via `tree_node_add_child()`
- **ActionPop**: Fills terminal nodes with PIF information
- **Father/Sibling Relations**: Established automatically when children are added
- **Output**: Table format showing all relations for easy inspection

The tree structure allows efficient traversal and represents the complete parse structure with all necessary information for semantic analysis.

