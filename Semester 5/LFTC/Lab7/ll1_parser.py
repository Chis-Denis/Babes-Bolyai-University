"""
Table-driven LL(1) parser implementation
"""

from typing import List, Optional, Tuple
from dataclasses import dataclass
from grammar_parser import Grammar, Production
from ll1_table import LL1Table


@dataclass
class ParseTreeNode:
    """Node in the parse tree"""
    node_id: int
    symbol: str  # Non-terminal or terminal
    production: Optional[Production] = None  # Production used (if non-terminal)
    children: List['ParseTreeNode'] = None
    parent: Optional['ParseTreeNode'] = None
    sibling: Optional['ParseTreeNode'] = None
    
    def __init__(self, node_id: int, symbol: str, production: Optional[Production] = None):
        self.node_id = node_id
        self.symbol = symbol
        self.production = production
        self.children = []
        self.parent = None
        self.sibling = None
    
    def add_child(self, child: 'ParseTreeNode'):
        """Add a child node"""
        child.parent = self
        if self.children:
            self.children[-1].sibling = child
        self.children.append(child)
        return child


class LL1Parser:
    """Table-driven LL(1) parser"""
    
    def __init__(self, grammar: Grammar, table: LL1Table):
        self.grammar = grammar
        self.table = table
        self.node_counter = 0
    
    def parse(self, input_tokens: List[str]) -> Tuple[Optional[ParseTreeNode], List[str]]:
        """
        Parse input tokens using LL(1) table.
        Returns (parse_tree_root, errors)
        """
        # Add $ at end
        tokens = input_tokens + ['$']
        token_index = 0
        
        # Initialize stack
        stack = []
        root = None
        current_node = None
        node_stack = []  # Stack to track parse tree nodes
        
        # Push start symbol
        if self.grammar.start_symbol:
            root = ParseTreeNode(self._next_id(), self.grammar.start_symbol)
            stack.append((self.grammar.start_symbol, root))
            node_stack.append(root)
        
        errors = []
        
        while stack:
            top_symbol, top_node = stack.pop()
            
            if top_symbol == tokens[token_index]:
                # Match terminal
                if top_node:
                    top_node.symbol = tokens[token_index]  # Update to actual token
                token_index += 1
            elif top_symbol in self.grammar.terminals:
                # Terminal mismatch - error
                errors.append(f"Expected '{top_symbol}' but found '{tokens[token_index]}'")
                # Try to recover by skipping token
                if token_index < len(tokens) - 1:
                    token_index += 1
            elif top_symbol in self.grammar.nonterminals:
                # Non-terminal - use table
                current_token = tokens[token_index]
                production = self.table.get_production(top_symbol, current_token)
                
                if not production:
                    # Error - no production in table
                    errors.append(f"No production for {top_symbol} on input '{current_token}'")
                    # Try to recover - use epsilon production if available, or skip
                    # Check if there's an epsilon production for this non-terminal
                    epsilon_prod = None
                    for prod in self.grammar.get_productions_for(top_symbol):
                        if not prod.right or prod.right == ['ε']:
                            # Check if current token is in FOLLOW
                            if current_token in self.table.follow_sets.get(top_symbol, set()):
                                epsilon_prod = prod
                                break
                    
                    if epsilon_prod:
                        top_node.production = epsilon_prod
                        eps_node = ParseTreeNode(self._next_id(), 'ε')
                        top_node.add_child(eps_node)
                        # Don't push anything, just continue
                        continue
                    else:
                        # Skip this non-terminal
                        continue
                
                # Apply production
                top_node.production = production
                rhs = production.right
                
                # Push right-hand side in reverse order
                if not rhs or rhs == ['ε']:
                    # Epsilon production - create epsilon node
                    eps_node = ParseTreeNode(self._next_id(), 'ε')
                    top_node.add_child(eps_node)
                else:
                    for symbol in reversed(rhs):
                        child_node = ParseTreeNode(self._next_id(), symbol)
                        top_node.add_child(child_node)
                        stack.append((symbol, child_node))
            else:
                # Unknown symbol
                errors.append(f"Unknown symbol: {top_symbol}")
        
        if token_index < len(tokens) - 1:
            errors.append(f"Input not fully consumed. Remaining: {tokens[token_index:]}")
        
        return root, errors
    
    def _next_id(self) -> int:
        """Get next node ID"""
        self.node_counter += 1
        return self.node_counter

