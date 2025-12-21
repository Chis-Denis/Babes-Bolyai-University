# Lab 5 - Predictive Parsing (LL-style) for the Mini DSL

## Overview
This lab builds a syntax analyzer (parser) on top of your existing FA-based scanner from Lab 4. The parser consumes the token stream and validates programs against the CFG defined in Lab 1, producing a parse tree and clear syntax errors.

## Layout
- `scanner.py` – Reusable scanner that tokenizes `program.txt` using FA from `Lab4/finite_automata.py` and `tokens.txt`
- `parser.py` – Recursive-descent parser for the Lab1 grammar, plus a simple AST/parse-tree writer
- `grammar.txt` – Human-readable CFG used by the parser

## How to Run
```bash
cd Lab5
python parser.py
```

Inputs:
- Uses `../Lab4/program.txt`
- Uses `../Lab4/tokens.txt`

Outputs (created in `Lab5/`):
- `parse_tree.txt` – Preorder, indented parse tree
- `SyntaxErrors.txt` – Syntax errors with line info (if any)

## Notes
- The scanner recognizes identifiers and constants using the FA from Lab 4 (identifiers, integers, strings, char literals). Keywords/operators come from `tokens.txt`.
- The parser implements a factorized form of the Lab1 grammar to avoid left recursion and allow predictive parsing.


