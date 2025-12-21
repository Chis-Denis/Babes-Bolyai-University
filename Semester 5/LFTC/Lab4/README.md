# Lab 4 - Finite Automata and Regular Grammars

## Overview
This lab implements Finite Automata (FA) and Regular Grammars (RG) for identifiers and integer constants, replacing the regex-based lexical analysis from Lab 3 with FA-based recognition.

## Requirements Completed
✓ Define Regular Grammar (RG) for identifiers and constants  
✓ Define Finite Automaton (FA) for identifiers and constants  
✓ Implement transformation from RG to FA (Theorem 1)  
✓ Check if input sequence is accepted by the FA  
✓ Integrate FA-based recognition into Lab 3 lexer  

## Files

### Source Code
- **finite_automata.py** - Core FA implementation:
  - `FiniteAutomaton` class with simulation logic
  - `rg_to_fa_identifier()` - Converts identifier grammar to FA
  - `rg_to_fa_integer()` - Converts integer grammar to FA
  - Test cases for both automata

- **lexer_fa.py** - Modified lexer using FA instead of regex:
  - `is_valid_identifier(token)` - Uses FA for identifier recognition
  - `is_valid_integer(token)` - Uses FA for integer recognition
  - Produces same output as Lab 3's regex-based lexer

### Documentation
- **documentation.txt** - Comprehensive explanation of:
  - Regular Grammar definitions
  - FA construction using Theorem 1
  - Implementation details
  - Testing results

## How to Run

### Test the Automata
```bash
cd Lab4
python finite_automata.py
```

### Run the FA-based Lexer
```bash
cd Lab4
python lexer_fa.py
```

This requires:
- `tokens.txt` (copied from Lab3)
- `program.txt` (copied from Lab3)

### Expected Outputs
- `PIF.txt` - Program Internal Form
- `ST.txt` - Symbol Table
- `Errors.txt` - Lexical errors

## Key Achievements

### 1. Regular Grammar Definition
**Identifier Grammar (G₁):**
```
S -> letter A | _
A -> letter A | digit A | _A
```

**Integer Grammar (G₂):**
```
S -> digit A
A -> digit A
```

### 2. Finite Automaton Construction (Theorem 1)
- States: Q = N ∪ {K}
- Initial state: q₀ = S
- Final states: F = {A, K}
- Transitions based on grammar productions

### 3. Verification
✓ FA-based lexer produces **identical output** to Lab 3's regex-based lexer  
✓ All test cases pass  
✓ Proven equivalence: Regex ⟷ RG ⟷ FA  

## Theoretical Background
Demonstrates the equivalence between:
- Regular expressions (Lab 3)
- Regular grammars (Lab 4)
- Finite automata (Lab 4)

This confirms **Theorem 1**: For any regular grammar G, there exists a Finite Automaton M such that L(G) = L(M).

---
*Lab 4 - Formal Languages & Compiler Design*

