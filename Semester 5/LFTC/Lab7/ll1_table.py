"""
Build LL(1) parsing table from FIRST and FOLLOW sets
"""

from typing import Dict, List, Tuple, Optional
from grammar_parser import Grammar, Production
from first_follow import compute_first_sets, compute_follow_sets, compute_first_of_sequence


class LL1Table:
    """LL(1) parsing table"""
    
    def __init__(self, grammar: Grammar):
        self.grammar = grammar
        self.first_sets = compute_first_sets(grammar)
        self.follow_sets = compute_follow_sets(grammar, self.first_sets)
        self.table: Dict[Tuple[str, str], Production] = {}
        self._build_table()
    
    def _build_table(self):
        """Build LL(1) parsing table"""
        # For each production A -> α
        for prod in self.grammar.productions:
            A = prod.left
            alpha = prod.right
            
            # Compute FIRST(α)
            first_alpha = compute_first_of_sequence(alpha, self.first_sets)
            
            # For each terminal a in FIRST(α) - {ε}
            for a in first_alpha - {'ε'}:
                key = (A, a)
                if key in self.table:
                    # Conflict - grammar is not LL(1)!
                    # Use first production found (or could use precedence)
                    print(f"WARNING: LL(1) conflict at table[{A}][{a}]")
                    print(f"  Existing: {self.table[key].index} ({self.table[key].left} -> {' '.join(self.table[key].right)})")
                    print(f"  New: {prod.index} ({prod.left} -> {' '.join(prod.right)})")
                    print(f"  Using existing production {self.table[key].index}")
                    # Keep existing, don't overwrite
                else:
                    self.table[key] = prod
            
            # If ε in FIRST(α), add production for all terminals in FOLLOW(A)
            if 'ε' in first_alpha:
                for b in self.follow_sets.get(A, set()):
                    key = (A, b)
                    if key in self.table:
                        # Conflict
                        print(f"WARNING: LL(1) conflict at table[{A}][{b}]")
                        print(f"  Existing: {self.table[key].index} ({self.table[key].left} -> {' '.join(self.table[key].right)})")
                        print(f"  New: {prod.index} ({prod.left} -> {' '.join(prod.right)})")
                        print(f"  Using existing production {self.table[key].index}")
                        # Keep existing, don't overwrite
                    else:
                        self.table[key] = prod
    
    def get_production(self, nonterminal: str, terminal: str) -> Optional[Production]:
        """Get production to use for nonterminal and terminal"""
        return self.table.get((nonterminal, terminal))
    
    def print_table(self):
        """Print the LL(1) parsing table"""
        print("\n" + "=" * 70)
        print("LL(1) Parsing Table")
        print("=" * 70)
        
        # Get all terminals
        terminals = sorted(self.grammar.terminals | {'$'})
        nonterminals = sorted(self.grammar.nonterminals)
        
        # Print header
        header = f"{'NT\\T':<15}"
        for t in terminals:
            header += f"{t[:8]:<10}"
        print(header)
        print("-" * (15 + 10 * len(terminals)))
        
        # Print rows
        for nt in nonterminals:
            row = f"{nt:<15}"
            for t in terminals:
                prod = self.table.get((nt, t))
                if prod:
                    rhs = ' '.join([s.replace('ε', 'eps') for s in prod.right[:3]]) if len(prod.right) > 3 else ' '.join([s.replace('ε', 'eps') for s in prod.right])
                    if len(rhs) > 8:
                        rhs = rhs[:5] + "..."
                    row += f"{str(prod.index):<10}"
                else:
                    row += f"{'':<10}"
            print(row)
        print()

