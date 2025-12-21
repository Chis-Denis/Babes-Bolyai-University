"""
Requirement 1: Test LL(1) parser with seminar grammar + sequence
Output: string of productions
"""

import os
from grammar_parser import Grammar
from first_follow import compute_first_sets, compute_follow_sets, print_first_follow
from ll1_table import LL1Table
from ll1_parser import LL1Parser
from parse_tree_table import get_productions_string

LAB_DIR = os.path.dirname(__file__)


def main():
    print("=" * 70)
    print("Lab 7 - Requirement 1: Seminar Grammar Test")
    print("=" * 70)
    print()
    
    # Load grammar
    grammar_file = os.path.join(LAB_DIR, "seminar_grammar.txt")
    grammar = Grammar()
    grammar.read_from_file(grammar_file)
    
    print("Grammar loaded:")
    print(grammar)
    print()
    
    # Compute FIRST and FOLLOW
    first_sets = compute_first_sets(grammar)
    follow_sets = compute_follow_sets(grammar, first_sets)
    print_first_follow(grammar, first_sets, follow_sets)
    
    # Build LL(1) table
    table = LL1Table(grammar)
    table.print_table()
    
    # Test input sequence
    # Example: id + id * id
    test_input = ['id', '+', 'id', '*', 'id']
    print(f"\nInput sequence: {' '.join(test_input)}")
    print()
    
    # Parse
    parser = LL1Parser(grammar, table)
    root, errors = parser.parse(test_input)
    
    if errors:
        print("Parsing errors:")
        for error in errors:
            print(f"  - {error}")
        print()
    
    if root:
        # Get productions string
        productions = get_productions_string(root)
        
        # Write to file
        output_file = os.path.join(LAB_DIR, "productions.txt")
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("String of Productions (Requirement 1)\n")
            f.write("=" * 70 + "\n\n")
            f.write(f"Input: {' '.join(test_input)}\n\n")
            f.write("Productions applied:\n")
            for i, prod in enumerate(productions, 1):
                f.write(f"{i}. {prod}\n")
        
        print("[OK] Parsing successful!")
        print(f"\nProductions written to: {output_file}")
        print("\nProductions applied:")
        for i, prod in enumerate(productions, 1):
            print(f"  {i}. {prod}")
    else:
        print("[ERROR] Parsing failed - no parse tree generated")


if __name__ == "__main__":
    main()

