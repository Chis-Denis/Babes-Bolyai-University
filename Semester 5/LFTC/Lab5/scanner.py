"""
Reusable scanner for Lab 5 that uses the FA from Lab 4.
Outputs a token stream consumed by the parser.

Token representation:
    Token(type, lexeme, line)

Where:
    - type is either a terminal from tokens.txt (e.g., 'int', 'OUTPUT', '+', '(', ...) or 'id'/'const'
    - lexeme is the original string (identifier name, numeric value, string literal, etc.)
"""

import os
import re
from dataclasses import dataclass
from typing import List

# Import finite automata from Lab4
LAB4_DIR = os.path.join(os.path.dirname(__file__), "..", "Lab4")
LAB4_DIR = os.path.abspath(LAB4_DIR)
import sys
sys.path.insert(0, LAB4_DIR)
from finite_automata import (
    rg_to_fa_identifier,
    rg_to_fa_integer,
    rg_to_fa_string,
    rg_to_fa_char_literal,
)


@dataclass
class Token:
    type: str
    lexeme: str
    line: int


def load_terminals() -> List[str]:
    tokens_path = os.path.join(LAB4_DIR, "tokens.txt")
    with open(tokens_path) as f:
        return [t.strip() for t in f.readlines() if t.strip()]


FA_ID = rg_to_fa_identifier()
FA_INT = rg_to_fa_integer()
FA_STR = rg_to_fa_string()
FA_CHAR = rg_to_fa_char_literal()


def scan_program(program_path: str) -> List[Token]:
    terminals = set(load_terminals())
    tokens: List[Token] = []

    with open(program_path) as src:
        for line_no, line in enumerate(src, start=1):
            # Space around punctuators for easier tokenization
            line = (
                line.replace("(", " ( ").replace(")", " ) ")
                    .replace("[", " [ ").replace("]", " ] ")
                    .replace(",", " , ")
            )

            # Tokenization regex (keeps strings/chars intact)
            raw = re.findall(
                r'"[^"\n]*"|\'[^\']*\'|@@|%%|\^\^|[0-9]+[a-zA-Z_]|[A-Za-z_][A-Za-z0-9_]*|[0-9]+|[><=]+|[^\s]',
                line,
            )

            for t in raw:
                if t in terminals:
                    tokens.append(Token(type=t, lexeme=t, line=line_no))
                elif FA_ID.accepts(t):
                    tokens.append(Token(type="id", lexeme=t, line=line_no))
                elif FA_INT.accepts(t) or FA_STR.accepts(t) or FA_CHAR.accepts(t):
                    tokens.append(Token(type="const", lexeme=t, line=line_no))
                elif re.match(r'^[0-9]+[a-zA-Z_]', t):
                    # invalid identifier like 1b – classify as error token; parser can report
                    tokens.append(Token(type="LEXERR", lexeme=t, line=line_no))
                else:
                    tokens.append(Token(type="LEXERR", lexeme=t, line=line_no))

    # Append EOF marker for the parser
    tokens.append(Token(type="$", lexeme="", line=line_no))
    return tokens


if __name__ == "__main__":
    program_path = os.path.join(LAB4_DIR, "program.txt")
    stream = scan_program(program_path)
    for tok in stream[:100]:
        print(tok)

