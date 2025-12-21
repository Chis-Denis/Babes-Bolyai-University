# Lab 7 - LL(1) Parsing with Parse Tree Table

## Overview
This lab implements LL(1) parsing algorithm from scratch (not using parser generators). It builds FIRST/FOLLOW sets, constructs an LL(1) parsing table, and outputs the parse tree as a table with father/sibling relations.

## Requirements Completed
✓ Requirement 1: Implement and test with seminar grammar + sequence → productions string  
✓ Requirement 2: Test with mini DSL grammar + PIF → Table with father/sibling relations (max 10 points)  

## Files

### Core Implementation
- **first_follow.py** - Computes FIRST and FOLLOW sets for grammar
- **ll1_table.py** - Builds LL(1) parsing table from FIRST/FOLLOW
- **ll1_parser.py** - Table-driven LL(1) parser implementation
- **parse_tree_table.py** - Converts parse tree to table format (father/sibling relations)

### Test Files
- **test_seminar.py** - Requirement 1: Test with seminar grammar
- **test_minidsl.py** - Requirement 2: Test with mini DSL + PIF
- **seminar_grammar.txt** - Example grammar from seminar

### Outputs
- **parse_tree_table.txt** - Parse tree as table (Node | Father | Sibling)
- **productions.txt** - String of productions (for Requirement 1)

## How to Run

### Requirement 1: Test with Seminar Grammar
```bash
cd Lab7
python test_seminar.py
```

### Requirement 2: Test with Mini DSL + PIF
```bash
cd Lab7
python test_minidsl.py
```

## Algorithm: LL(1)

1. **Compute FIRST sets** - For each non-terminal, find terminals that can start its derivations
2. **Compute FOLLOW sets** - For each non-terminal, find terminals that can follow it
3. **Build LL(1) table** - Fill table[nonterminal][terminal] with production to use
4. **Parse using table** - Use table to guide parsing decisions
5. **Build parse tree** - Track parent/child/sibling relationships
6. **Output as table** - Format parse tree as Node | Father | Sibling table

## Parse Tree Table Format

```
Node | Father | Sibling
-----|--------|--------
1    | 0      | 2      (Node 1, root, sibling is 2)
2    | 1      | 0      (Node 2, child of 1, no sibling)
...
```

---
*Lab 7 - Formal Languages & Compiler Design*

