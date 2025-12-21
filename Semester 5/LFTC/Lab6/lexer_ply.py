"""
PLY Lexer for Lab 6
Can read from PIF (Lab 4) or use integrated tokenization
"""

import os
import re
import sys
from typing import List, Iterator

# Import FA from Lab 4 for token recognition
LAB4_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "Lab4"))
sys.path.insert(0, LAB4_DIR)
from finite_automata import (
    rg_to_fa_identifier,
    rg_to_fa_integer,
    rg_to_fa_string,
    rg_to_fa_char_literal,
)

# Create FA instances
FA_ID = rg_to_fa_identifier()
FA_INT = rg_to_fa_integer()
FA_STR = rg_to_fa_string()
FA_CHAR = rg_to_fa_char_literal()

# Load terminals from Lab 4
def load_terminals():
    tokens_path = os.path.join(LAB4_DIR, "tokens.txt")
    with open(tokens_path) as f:
        return [t.strip() for t in f.readlines() if t.strip()]

TERMINALS = set(load_terminals())

# Token names for PLY
tokens = (
    # Keywords
    'INT', 'STR', 'CHAR', 'ARRAY_INT', 'ARRAY_STR',
    'IF', 'THEN', 'ELSE', 'END', 'WHILE', 'DO',
    'INPUT', 'OUTPUT', 'DEF', 'BEGIN', 'RETURN',
    # Operators
    'PLUS', 'MINUS', 'MULT', 'DIV', 'POW', 'LCM', 'POWER_SUM',
    'EQ', 'NE', 'GT', 'LT', 'GE', 'LE',
    # Identifiers and constants
    'ID', 'CONST',
    # Delimiters
    'LPAREN', 'RPAREN', 'LBRACKET', 'RBRACKET', 'COMMA', 'ASSIGN',
)

# PLY token definitions (regex patterns)
def t_ARRAY_INT(t):
    r'array<int>'
    return t

def t_ARRAY_STR(t):
    r'array<str>'
    return t

def t_POWER_SUM(t):
    r'@@'
    return t

def t_LCM(t):
    r'%%'
    return t

def t_POW(t):
    r'\^\^'
    return t

def t_GE(t):
    r'>='
    return t

def t_LE(t):
    r'<='
    return t

def t_NE(t):
    r'!='
    return t

def t_EQ(t):
    r'=='
    return t

def t_GT(t):
    r'>'
    return t

def t_LT(t):
    r'<'
    return t

def t_PLUS(t):
    r'\+'
    return t

def t_MINUS(t):
    r'-'
    return t

def t_MULT(t):
    r'\*'
    return t

def t_DIV(t):
    r'/'
    return t

def t_LPAREN(t):
    r'\('
    return t

def t_RPAREN(t):
    r'\)'
    return t

def t_LBRACKET(t):
    r'\['
    return t

def t_RBRACKET(t):
    r'\]'
    return t

def t_COMMA(t):
    r','
    return t

def t_ASSIGN(t):
    r'='
    return t

def t_ID(t):
    r'[A-Za-z_][A-Za-z0-9_]*'
    # Verify with FA
    if t.value in TERMINALS:
        # Map keywords to token types
        keyword_map = {
            'int': 'INT', 'str': 'STR', 'char': 'CHAR',
            'IF': 'IF', 'THEN': 'THEN', 'ELSE': 'ELSE', 'END': 'END',
            'WHILE': 'WHILE', 'DO': 'DO',
            'INPUT': 'INPUT', 'OUTPUT': 'OUTPUT',
            'DEF': 'DEF', 'BEGIN': 'BEGIN', 'RETURN': 'RETURN',
        }
        if t.value in keyword_map:
            t.type = keyword_map[t.value]
            return t
    elif FA_ID.accepts(t.value):
        t.type = 'ID'
        return t
    return None

def t_CONST(t):
    r'"[^"]*"|\'[^\']*\'|[0-9]+'
    # Check with FA
    if FA_STR.accepts(t.value) or FA_CHAR.accepts(t.value):
        t.type = 'CONST'
        return t
    elif FA_INT.accepts(t.value):
        t.type = 'CONST'
        return t
    return None

def t_WHITESPACE(t):
    r'\s+'
    pass  # Ignore whitespace

def t_error(t):
    print(f"Illegal character '{t.value[0]}' at line {t.lineno}")
    t.lexer.skip(1)

# Build lexer
import ply.lex as lex
lexer = lex.lex()


def read_pif_tokens(pif_path: str) -> Iterator[tuple]:
    """
    Read tokens from PIF file (Lab 4 format).
    Returns iterator of (token_type, lexeme) tuples.
    """
    with open(pif_path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            # Parse PIF format: ('token', index) or ('id', index) or ('const', index)
            import ast
            try:
                entry = ast.literal_eval(line)
                if isinstance(entry, tuple) and len(entry) == 2:
                    token_type, idx = entry
                    if idx == -1:
                        # Keyword/operator - use token_type as is
                        yield (token_type, token_type)
                    else:
                        # id or const - need to look up lexeme from ST
                        yield (token_type, None)  # Will need ST lookup
            except:
                pass


def tokenize_from_pif(pif_path: str, st_path: str) -> List:
    """
    Convert PIF to token stream for PLY parser.
    Reads PIF and ST to reconstruct token stream.
    """
    # Read symbol table
    st = {"identifiers": {}, "constants": {}}
    with open(st_path) as f:
        current = None
        for line in f:
            line = line.strip()
            if line == "Identifiers:":
                current = "identifiers"
            elif line == "Constants:":
                current = "constants"
            elif line and current:
                parts = line.split(" -> ")
                if len(parts) == 2:
                    idx, value = parts
                    st[current][int(idx)] = value
    
    # Read PIF and convert to tokens
    tokens = []
    with open(pif_path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            import ast
            try:
                entry = ast.literal_eval(line)
                if isinstance(entry, tuple) and len(entry) == 2:
                    token_type, idx = entry
                    if idx == -1:
                        # Keyword/operator
                        tokens.append((token_type, token_type))
                    elif token_type == "id":
                        # Identifier - lookup from ST
                        lexeme = st["identifiers"].get(idx, f"id_{idx}")
                        tokens.append(("ID", lexeme))
                    elif token_type == "const":
                        # Constant - lookup from ST
                        lexeme = st["constants"].get(idx, f"const_{idx}")
                        tokens.append(("CONST", lexeme))
            except:
                pass
    
    return tokens


if __name__ == "__main__":
    # Test lexer
    test_input = 'int a = 5'
    lexer.input(test_input)
    for tok in lexer:
        print(tok)

