"""
Grammar parser - reads grammar from file and builds data structures
"""

from typing import Dict, List, Set, Tuple, Optional
from dataclasses import dataclass


@dataclass
class Production:
    """Represents a grammar production"""
    left: str  # Non-terminal
    right: List[str]  # Right-hand side (list of symbols)
    index: int  # Production number


class Grammar:
    """Represents a context-free grammar"""
    
    def __init__(self):
        self.productions: List[Production] = []
        self.nonterminals: Set[str] = set()
        self.terminals: Set[str] = set()
        self.start_symbol: Optional[str] = None
        
    def add_production(self, left: str, right: List[str], index: int = None):
        """Add a production to the grammar"""
        if index is None:
            index = len(self.productions)
        prod = Production(left, right, index)
        self.productions.append(prod)
        self.nonterminals.add(left)
        for symbol in right:
            if symbol and symbol != 'ε':
                if symbol[0].isupper() or symbol in ['ε']:
                    # Could be non-terminal or epsilon
                    if symbol not in ['ε'] and not self._is_terminal(symbol):
                        self.nonterminals.add(symbol)
                else:
                    self.terminals.add(symbol)
    
    def _is_terminal(self, symbol: str) -> bool:
        """Heuristic: terminals are keywords, operators, or special symbols"""
        if symbol in ['ε', '']:
            return False
        if symbol.startswith('"') or symbol.startswith("'"):
            return True
        if symbol in ['id', 'const']:  # Special terminals
            return True
        
        # Known keywords (from Lab4/tokens.txt)
        keywords = ['int', 'str', 'char', 'IF', 'THEN', 'ELSE', 'WHILE', 'DO', 
                   'DEF', 'BEGIN', 'END', 'RETURN', 'INPUT', 'OUTPUT']
        if symbol in keywords:
            return True
        
        # Operators and special symbols
        operators = ['+', '-', '*', '/', '=', '==', '!=', '>', '<', '>=', '<=',
                    '(', ')', '[', ']', ',', '^^', '%%', '@@']
        if symbol in operators:
            return True
        
        # If it's all uppercase (likely keyword) or starts with special char
        if symbol.isupper() or (symbol and not symbol[0].isalpha()):
            return True
        
        # Non-terminals are typically Capitalized (first letter uppercase, rest lowercase)
        # or multi-word like "StmtList", "DeclStmt"
        if symbol and symbol[0].isupper() and len(symbol) > 1:
            # Check if it looks like a non-terminal pattern
            if symbol[1:].islower() or any(c.isupper() for c in symbol[1:]):
                # Could be non-terminal (e.g., "StmtList", "DeclStmt")
                return False
        
        # Default: lowercase or short symbols are terminals
        return symbol.islower() or len(symbol) <= 2
    
    def get_productions_for(self, nonterminal: str) -> List[Production]:
        """Get all productions for a given non-terminal"""
        return [p for p in self.productions if p.left == nonterminal]
    
    def read_from_file(self, filename: str):
        """Read grammar from file (BNF-like format)"""
        with open(filename, 'r', encoding='utf-8') as f:
            lines = f.readlines()
        
        index = 0
        for line in lines:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            
            # Parse: A -> B C | D
            if '->' in line:
                parts = line.split('->', 1)
                left = parts[0].strip()
                right_part = parts[1].strip()
                
                if not self.start_symbol:
                    self.start_symbol = left
                
                # Handle alternatives (|)
                alternatives = right_part.split('|')
                for alt in alternatives:
                    alt = alt.strip()
                    if alt == 'ε' or alt == 'epsilon':
                        right = ['ε']
                    else:
                        # Split by spaces, but keep quoted strings together
                        right = self._tokenize_rhs(alt)
                    
                    self.add_production(left, right, index)
                    index += 1
    
    def _tokenize_rhs(self, rhs: str) -> List[str]:
        """Tokenize right-hand side, preserving quoted strings"""
        tokens = []
        current = ""
        in_quote = False
        quote_char = None
        
        for char in rhs:
            if char in ['"', "'"] and not in_quote:
                in_quote = True
                quote_char = char
                current += char
            elif char == quote_char and in_quote:
                in_quote = False
                current += char
                tokens.append(current)
                current = ""
            elif in_quote:
                current += char
            elif char.isspace():
                if current:
                    tokens.append(current)
                    current = ""
            else:
                current += char
        
        if current:
            tokens.append(current)
        
        return [t for t in tokens if t]
    
    def __str__(self):
        result = f"Grammar (Start: {self.start_symbol})\n"
        result += f"Non-terminals: {sorted(self.nonterminals)}\n"
        result += f"Terminals: {sorted(self.terminals)}\n\n"
        result += "Productions:\n"
        for p in self.productions:
            rhs = ' '.join(p.right) if p.right else 'ε'
            result += f"  {p.index}. {p.left} -> {rhs}\n"
        return result

