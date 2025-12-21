"""
Requirement 2: Test LL(1) parser with mini DSL grammar + PIF
Output: Table with father/sibling relations (max 10 points)
"""

import os
from grammar_parser import Grammar
from first_follow import compute_first_sets, compute_follow_sets, print_first_follow
from ll1_table import LL1Table
from ll1_parser import LL1Parser
from parse_tree_table import build_tree_table, print_tree_table
from pif_reader import read_pif_and_st

LAB_DIR = os.path.dirname(__file__)
LAB4_DIR = os.path.abspath(os.path.join(LAB_DIR, "..", "Lab4"))


def main():
    print("=" * 70)
    print("Lab 7 - Requirement 2: Mini DSL Grammar + PIF Test")
    print("=" * 70)
    print()
    
    # Load grammar
    grammar_file = os.path.join(LAB_DIR, "minidsl_grammar.txt")
    grammar = Grammar()
    grammar.read_from_file(grammar_file)
    
    print("Grammar loaded:")
    print(f"  Start symbol: {grammar.start_symbol}")
    print(f"  Non-terminals: {len(grammar.nonterminals)}")
    print(f"  Terminals: {len(grammar.terminals)}")
    print(f"  Productions: {len(grammar.productions)}")
    print()
    
    # Compute FIRST and FOLLOW
    print("Computing FIRST and FOLLOW sets...")
    first_sets = compute_first_sets(grammar)
    follow_sets = compute_follow_sets(grammar, first_sets)
    print_first_follow(grammar, first_sets, follow_sets)
    
    # Build LL(1) table
    print("Building LL(1) parsing table...")
    table = LL1Table(grammar)
    table.print_table()
    
    # Read PIF and ST from Lab 4
    pif_path = os.path.join(LAB4_DIR, "PIF.txt")
    st_path = os.path.join(LAB4_DIR, "ST.txt")
    
    if not os.path.exists(pif_path):
        print(f"ERROR: PIF file not found: {pif_path}")
        print("Please run Lab 4 lexer first to generate PIF.txt")
        return
    
    if not os.path.exists(st_path):
        print(f"ERROR: ST file not found: {st_path}")
        print("Please run Lab 4 lexer first to generate ST.txt")
        return
    
    print(f"Reading PIF from: {pif_path}")
    print(f"Reading ST from: {st_path}")
    
    input_tokens = read_pif_and_st(pif_path, st_path)
    print(f"\nInput token stream ({len(input_tokens)} tokens):")
    print(f"  {' '.join(input_tokens[:50])}{'...' if len(input_tokens) > 50 else ''}")
    print()
    
    # Parse
    print("Parsing...")
    parser = LL1Parser(grammar, table)
    root, errors = parser.parse(input_tokens)
    
    if errors:
        print("\nParsing errors:")
        for error in errors:
            print(f"  - {error}")
        print()
    
    if root:
        print("[OK] Parsing successful!")
        
        # Build parse tree table
        print("\nBuilding parse tree table...")
        table_data = build_tree_table(root)
        
        # Write to file
        output_file = os.path.join(LAB_DIR, "parse_tree_table.txt")
        print_tree_table(table_data, output_file)
        
        print(f"\n[OK] Parse tree table written to: {output_file}")
        print("\nThis table shows the parse tree structure with:")
        print("  - Node: Node ID")
        print("  - Symbol: Grammar symbol (non-terminal or terminal)")
        print("  - Father: Parent node ID (0 for root)")
        print("  - Sibling: Next sibling node ID (0 if none)")
        print("  - Production: Production number used (for non-terminals)")
    else:
        print("[ERROR] Parsing failed - no parse tree generated")
        if not errors:
            print("  (No errors reported, but no tree was built)")


if __name__ == "__main__":
    main()

