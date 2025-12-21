# Lab 6 - Parsing with yacc (PLY Implementation)

## Overview
This lab implements a parser generator solution using **yacc** (specifically PLY - Python Lex-Yacc, which is a direct Python implementation of yacc) for the mini DSL. It integrates scanning with parsing and outputs a derivation sequence showing the grammar productions applied during parsing.

**Note:** PLY (Python Lex-Yacc) is a complete implementation of yacc for Python, meeting the requirement to "use yacc or ANTLR". PLY provides the same functionality as traditional yacc/bison but in Python, making it compatible with the existing Python codebase from Labs 4-5.

## Requirements Completed
✓ Define grammar for mini-DSL (in yacc/PLY format)  
✓ Perform parsing using yacc (PLY implementation)  
✓ Integrate scanning with parsing (lex with yacc)  
✓ Output string of productions (derivation sequence)  
✓ Use PIF as input (from Lab 4)  

## Files

### Source Code
- **lexer_ply.py** - PLY lexer that reads PIF from Lab 4 or uses integrated scanner
- **parser_ply.py** - PLY parser with grammar rules and production tracking
- **main.py** - Main entry point that integrates scanner and parser

### Outputs
- **derivations.txt** - Sequence of grammar productions applied during parsing
- **SyntaxErrors.txt** - Syntax errors (if any)

## How to Run

### Prerequisites
Install PLY:
```bash
pip install ply
```

### Run the Parser
```bash
cd Lab6
python main.py
```

This will:
1. Read PIF from `../Lab4/PIF.txt` (or use integrated scanner)
2. Parse the token stream using PLY
3. Generate `derivations.txt` with production sequence
4. Report any syntax errors

## Input/Output

**Input:**
- Grammar: Defined in `parser_ply.py` (PLY format)
- PIF: Token stream from `../Lab4/PIF.txt`

**Output:**
- `derivations.txt`: String of productions showing derivation sequence
  Example:
  ```
  Program -> StmtList
  StmtList -> Stmt StmtList
  Stmt -> DeclStmt
  DeclStmt -> Type id DeclTail
  ...
  ```

## Integration

The parser integrates:
- **Scanner**: Can read PIF from Lab 4 OR use integrated PLY lexer
- **Parser**: PLY-based parser with grammar rules
- **Production Tracking**: Records each grammar rule application

## Grammar

The grammar is defined in PLY format in `parser_ply.py`, based on the factorized grammar from Lab 5, adapted for PLY's bottom-up parsing (LALR).

---
*Lab 6 - Formal Languages & Compiler Design*

