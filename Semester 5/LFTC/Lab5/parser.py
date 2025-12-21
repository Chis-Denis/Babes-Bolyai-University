"""
Recursive-descent parser for the Lab1 mini DSL.

Integration:
  - Uses Lab5/scanner.py to produce a token stream from Lab4/program.txt
  - Writes parse_tree.txt and SyntaxErrors.txt in Lab5/

The grammar is a factorized, LL-style version found in grammar.txt.
"""

import os
from typing import List, Optional
from dataclasses import dataclass

from scanner import scan_program, Token


LAB_DIR = os.path.dirname(__file__)
LAB4_DIR = os.path.abspath(os.path.join(LAB_DIR, "..", "Lab4"))


@dataclass
class Node:
    name: str
    children: List["Node"]
    token: Optional[Token] = None

    def add(self, child: "Node"):
        self.children.append(child)
        return child


class Parser:
    def __init__(self, tokens: List[Token]):
        self.tokens = tokens
        self.pos = 0
        self.errors: List[str] = []

    # Utility
    def look(self) -> Token:
        return self.tokens[self.pos]

    def advance(self) -> Token:
        t = self.tokens[self.pos]
        self.pos += 1
        return t

    def accept(self, ttype: str) -> Optional[Token]:
        if self.look().type == ttype:
            return self.advance()
        return None

    def expect(self, ttype: str) -> Token:
        tok = self.look()
        if tok.type == ttype:
            return self.advance()
        self.error(f"Expected '{ttype}' but found '{tok.type}' at line {tok.line}")
        # Attempt to continue to avoid infinite loops
        return self.advance()

    def error(self, msg: str):
        self.errors.append(msg)

    # Parsing functions correspond to grammar.txt
    def parse_program(self) -> Node:
        root = Node("Program", [])
        root.add(self.parse_stmt_list())
        self.expect("$")
        return root

    def parse_stmt_list(self) -> Node:
        node = Node("StmtList", [])
        while True:
            la = self.look().type
            if la in {"int", "str", "char", "array<int>", "array<str>", "id", "INPUT", "OUTPUT", "IF", "WHILE", "(", "const"}:
                node.add(self.parse_stmt())
                continue
            break
        return node

    def parse_stmt(self) -> Node:
        la = self.look().type
        node = Node("Stmt", [])
        if la in {"int", "str", "char", "array<int>", "array<str>"}:
            node.add(self.parse_decl_stmt())
        elif la == "id":
            # Could be assign, array access assign, func call, or expr stmt. Try assign first.
            # Lookahead for '=' or '[' or '(' to disambiguate
            if self._peek_is(1, "="):
                node.add(self.parse_assign_stmt())
            else:
                node.add(self.parse_expr_stmt())
        elif la == "INPUT":
            node.add(self.parse_input_stmt())
        elif la == "OUTPUT":
            node.add(self.parse_output_stmt())
        elif la == "IF":
            node.add(self.parse_if_stmt())
        elif la == "WHILE":
            node.add(self.parse_while_stmt())
        else:
            self.error(f"Unexpected token '{la}' at line {self.look().line} in statement")
            self.advance()
        return node

    def parse_decl_stmt(self) -> Node:
        node = Node("DeclStmt", [])
        node.add(self.parse_type())
        node.add(Node("id", [], self.expect("id")))
        if self.accept("="):
            node.add(self.parse_expr())
        return node

    def parse_type(self) -> Node:
        node = Node("Type", [])
        la = self.look().type
        if la in {"int", "str", "char", "array<int>", "array<str>"}:
            node.token = self.advance()
        else:
            self.error(f"Type expected at line {self.look().line}")
            self.advance()
        return node

    def parse_assign_stmt(self) -> Node:
        node = Node("AssignStmt", [])
        node.add(Node("id", [], self.expect("id")))
        self.expect("=")
        node.add(self.parse_expr())
        return node

    def parse_input_stmt(self) -> Node:
        node = Node("InputStmt", [])
        self.expect("INPUT")
        node.add(Node("id", [], self.expect("id")))
        return node

    def parse_output_stmt(self) -> Node:
        node = Node("OutputStmt", [])
        self.expect("OUTPUT")
        node.add(self.parse_expr())
        return node

    def parse_if_stmt(self) -> Node:
        node = Node("IfStmt", [])
        self.expect("IF")
        node.add(self.parse_cond())
        self.expect("THEN")
        node.add(self.parse_stmt_list())
        if self.accept("ELSE"):
            node.add(self.parse_stmt_list())
        self.expect("END")
        return node

    def parse_while_stmt(self) -> Node:
        node = Node("WhileStmt", [])
        self.expect("WHILE")
        node.add(self.parse_cond())
        self.expect("DO")
        node.add(self.parse_stmt_list())
        self.expect("END")
        return node

    def parse_expr_stmt(self) -> Node:
        node = Node("ExprStmt", [])
        node.add(self.parse_expr())
        return node

    # Expressions with precedence
    def parse_expr(self) -> Node:
        node = Node("Expr", [])
        node.add(self.parse_term())
        while self.look().type in {"+", "-"}:
            node.add(Node("op", [], self.advance()))
            node.add(self.parse_term())
        return node

    def parse_term(self) -> Node:
        node = Node("Term", [])
        node.add(self.parse_power())
        while self.look().type in {"*", "/"}:
            node.add(Node("op", [], self.advance()))
            node.add(self.parse_power())
        return node

    def parse_power(self) -> Node:
        node = Node("Power", [])
        node.add(self.parse_factor())
        while self.look().type in {"^^", "%%", "@@"}:
            node.add(Node("op", [], self.advance()))
            node.add(self.parse_factor())
        return node

    def parse_factor(self) -> Node:
        la = self.look().type
        node = Node("Factor", [])
        if la == "id":
            # disambiguate: array access / func call / plain id
            if self._peek_is(1, "("):
                node.add(self.parse_func_call())
            elif self._peek_is(1, "["):
                node.add(self.parse_array_access())
            else:
                node.add(Node("id", [], self.advance()))
        elif la == "const":
            node.add(Node("const", [], self.advance()))
        elif la == "(":
            self.advance()
            node.add(self.parse_expr())
            self.expect(")")
        else:
            self.error(f"Unexpected token in factor: '{la}' at line {self.look().line}")
            self.advance()
        return node

    def parse_array_access(self) -> Node:
        node = Node("ArrayAccess", [])
        node.add(Node("id", [], self.expect("id")))
        self.expect("[")
        node.add(self.parse_expr())
        self.expect("]")
        return node

    def parse_func_call(self) -> Node:
        node = Node("FuncCall", [])
        node.add(Node("id", [], self.expect("id")))
        self.expect("(")
        if self.look().type != ")":
            node.add(self.parse_expr())
            while self.accept(","):
                node.add(self.parse_expr())
        self.expect(")")
        return node

    def parse_cond(self) -> Node:
        node = Node("Cond", [])
        node.add(self.parse_expr())
        cmp_ops = {"=", "!=", ">", "<", ">=", "<="}
        if self.look().type in cmp_ops:
            node.add(Node("cmp", [], self.advance()))
        else:
            self.error(f"Comparison operator expected at line {self.look().line}")
        node.add(self.parse_expr())
        return node

    def _peek_is(self, k: int, ttype: str) -> bool:
        idx = self.pos + k
        if idx < len(self.tokens):
            return self.tokens[idx].type == ttype
        return False


def write_tree(node: Node, path: str, indent: int = 0, fh=None):
    close_after = False
    if fh is None:
        fh = open(path, "w", encoding="utf-8")
        close_after = True
    pad = "  " * indent
    label = node.name
    if node.token is not None:
        label += f" [{node.token.type}:{node.token.lexeme}]"
    fh.write(pad + label + "\n")
    for c in node.children:
        write_tree(c, path, indent + 1, fh)
    if close_after:
        fh.close()


def main():
    program_path = os.path.join(LAB4_DIR, "program.txt")
    tokens = scan_program(program_path)

    # Basic lexical error check before parsing
    lex_errs = [t for t in tokens if t.type == "LEXERR"]
    syntax_err_path = os.path.join(LAB_DIR, "SyntaxErrors.txt")
    with open(syntax_err_path, "w", encoding="utf-8") as ferr:
        for e in lex_errs:
            ferr.write(f"Lexical error at line {e.line}: {e.lexeme}\n")

    parser = Parser(tokens)
    tree = parser.parse_program()

    # Write syntax errors (if any)
    with open(syntax_err_path, "a", encoding="utf-8") as ferr:
        for msg in parser.errors:
            ferr.write(msg + "\n")

    tree_path = os.path.join(LAB_DIR, "parse_tree.txt")
    write_tree(tree, tree_path)

    print("Parsing complete.")
    if parser.errors or lex_errs:
        print(f"Errors written to {os.path.basename(syntax_err_path)}")
    else:
        print("No syntax errors detected.")
    print(f"Parse tree written to {os.path.basename(tree_path)}")


if __name__ == "__main__":
    main()


