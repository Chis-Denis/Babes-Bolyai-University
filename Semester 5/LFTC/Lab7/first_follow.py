"""
Compute FIRST and FOLLOW sets for LL(1) parsing
"""

from typing import Dict, Set, List
from grammar_parser import Grammar, Production


def compute_first_sets(grammar: Grammar) -> Dict[str, Set[str]]:
    """
    Compute FIRST sets for all symbols in the grammar.
    FIRST(X) = set of terminals that can start strings derived from X
    """
    first = {}
    
    # Initialize FIRST sets
    for nt in grammar.nonterminals:
        first[nt] = set()
    for t in grammar.terminals:
        first[t] = {t}  # Terminal's FIRST is itself
    first['ε'] = {'ε'}
    
    # Iterate until no changes
    changed = True
    while changed:
        changed = False
        
        for prod in grammar.productions:
            left = prod.left
            right = prod.right
            
            if not right or right == ['ε']:
                # X -> ε: add ε to FIRST(X)
                if 'ε' not in first[left]:
                    first[left].add('ε')
                    changed = True
            else:
                # X -> Y1 Y2 ... Yk
                # Add FIRST(Y1) - {ε} to FIRST(X)
                # If ε in FIRST(Y1), add FIRST(Y2) - {ε}, etc.
                all_have_epsilon = True
                for i, symbol in enumerate(right):
                    if symbol not in first:
                        # Unknown symbol, skip
                        all_have_epsilon = False
                        break
                    
                    # Add FIRST(symbol) - {ε} to FIRST(left)
                    first_symbol = first.get(symbol, set())
                    before_size = len(first[left])
                    first[left].update(first_symbol - {'ε'})
                    if len(first[left]) > before_size:
                        changed = True
                    
                    if 'ε' not in first_symbol:
                        all_have_epsilon = False
                        break
                
                # If all symbols can derive ε, add ε to FIRST(left)
                if all_have_epsilon and 'ε' not in first[left]:
                    first[left].add('ε')
                    changed = True
    
    return first


def compute_follow_sets(grammar: Grammar, first_sets: Dict[str, Set[str]]) -> Dict[str, Set[str]]:
    """
    Compute FOLLOW sets for all non-terminals.
    FOLLOW(A) = set of terminals that can appear immediately after A
    """
    follow = {}
    
    # Initialize FOLLOW sets
    for nt in grammar.nonterminals:
        follow[nt] = set()
    
    # Add $ to FOLLOW of start symbol
    if grammar.start_symbol:
        follow[grammar.start_symbol].add('$')
    
    # Iterate until no changes
    changed = True
    while changed:
        changed = False
        
        for prod in grammar.productions:
            left = prod.left
            right = prod.right
            
            # For each production A -> αBβ
            for i in range(len(right)):
                B = right[i]
                if B not in grammar.nonterminals:
                    continue
                
                # β is the suffix after B
                beta = right[i+1:] if i+1 < len(right) else []
                
                if not beta:
                    # A -> αB: add FOLLOW(A) to FOLLOW(B)
                    before_size = len(follow[B])
                    follow[B].update(follow[left])
                    if len(follow[B]) > before_size:
                        changed = True
                else:
                    # A -> αBβ: add FIRST(β) - {ε} to FOLLOW(B)
                    first_beta = compute_first_of_sequence(beta, first_sets)
                    before_size = len(follow[B])
                    follow[B].update(first_beta - {'ε'})
                    if len(follow[B]) > before_size:
                        changed = True
                    
                    # If ε in FIRST(β), add FOLLOW(A) to FOLLOW(B)
                    if 'ε' in first_beta:
                        before_size = len(follow[B])
                        follow[B].update(follow[left])
                        if len(follow[B]) > before_size:
                            changed = True
    
    return follow


def compute_first_of_sequence(sequence: List[str], first_sets: Dict[str, Set[str]]) -> Set[str]:
    """Compute FIRST set for a sequence of symbols"""
    if not sequence:
        return {'ε'}
    
    result = set()
    all_have_epsilon = True
    
    for symbol in sequence:
        first_symbol = first_sets.get(symbol, set())
        result.update(first_symbol - {'ε'})
        
        if 'ε' not in first_symbol:
            all_have_epsilon = False
            break
    
    if all_have_epsilon:
        result.add('ε')
    
    return result


def print_first_follow(grammar: Grammar, first_sets: Dict[str, Set[str]], follow_sets: Dict[str, Set[str]]):
    """Print FIRST and FOLLOW sets in readable format"""
    def safe_str(s):
        """Convert epsilon to 'epsilon' for Windows console compatibility"""
        return s.replace('ε', 'epsilon')
    
    print("\n" + "=" * 70)
    print("FIRST Sets")
    print("=" * 70)
    for nt in sorted(grammar.nonterminals):
        first_str = ', '.join(sorted([safe_str(s) for s in first_sets.get(nt, set())]))
        print(f"FIRST({nt}) = {{{first_str}}}")
    
    print("\n" + "=" * 70)
    print("FOLLOW Sets")
    print("=" * 70)
    for nt in sorted(grammar.nonterminals):
        follow_str = ', '.join(sorted([safe_str(s) for s in follow_sets.get(nt, set())]))
        print(f"FOLLOW({nt}) = {{{follow_str}}}")
    print()

