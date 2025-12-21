"""
Lab 6 - Main entry point
Integrates scanning with parsing using PLY
Reads PIF from Lab 4 and outputs derivation sequence
"""

import os
import sys
import ast
from lexer_ply import tokenize_from_pif
from parser_ply import parser, get_productions, reset_productions
import lexer_ply

LAB_DIR = os.path.dirname(__file__)
LAB4_DIR = os.path.abspath(os.path.join(LAB_DIR, "..", "Lab4"))


def convert_pif_to_ply_input(pif_path: str, st_path: str) -> str:
    """
    Convert PIF tokens to a string that PLY lexer can process.
    This integrates the scanner (PIF) with the parser.
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
    
    # Read PIF and convert to string
    token_strings = []
    with open(pif_path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                entry = ast.literal_eval(line)
                if isinstance(entry, tuple) and len(entry) == 2:
                    token_type, idx = entry
                    if idx == -1:
                        # Keyword/operator - use as-is
                        token_strings.append(token_type)
                    elif token_type == "id":
                        # Identifier - lookup from ST
                        lexeme = st["identifiers"].get(idx, f"id_{idx}")
                        token_strings.append(lexeme)
                    elif token_type == "const":
                        # Constant - lookup from ST
                        lexeme = st["constants"].get(idx, f"const_{idx}")
                        token_strings.append(lexeme)
            except:
                pass
    
    return ' '.join(token_strings)


def main():
    """Main function: integrate scanning with parsing"""
    
    # Option 1: Read from program.txt directly (integrated scanner)
    program_path = os.path.join(LAB4_DIR, "program.txt")
    
    # Option 2: Read from PIF (alternative approach)
    pif_path = os.path.join(LAB4_DIR, "PIF.txt")
    st_path = os.path.join(LAB4_DIR, "ST.txt")
    
    print("=" * 60)
    print("Lab 6 - Parsing with PLY (Scanner + Parser Integration)")
    print("=" * 60)
    
    # Use PIF as input (as per requirements: grammar + PIF)
    if os.path.exists(pif_path) and os.path.exists(st_path):
        print(f"Reading PIF from: {pif_path}")
        print(f"Reading ST from: {st_path}")
        print("Converting PIF token stream for parser...")
        print()
        
        input_string = convert_pif_to_ply_input(pif_path, st_path)
        print(f"Token stream from PIF: {input_string[:80]}...")
        print()
    elif os.path.exists(program_path):
        # Fallback: use program.txt directly (for testing)
        print(f"Note: Using program.txt directly (PIF not found)")
        print(f"Reading program from: {program_path}")
        print()
        
        with open(program_path, 'r') as f:
            input_string = f.read()
        
        print(f"Input program:\n{input_string}")
        print()
    else:
        print("Error: Neither PIF.txt nor program.txt found")
        return
    
    # Step 1: Parse using PLY (Scanner + Parser integrated)
    print("Step 1: Parsing with PLY (integrated scanner + parser)...")
    reset_productions()  # Clear previous productions
    
    try:
        result = parser.parse(input_string, lexer=lexer_ply.lexer, debug=False)
        print("Parsing successful!")
        print()
    except Exception as e:
        print(f"Parsing error: {e}")
        import traceback
        traceback.print_exc()
        result = None
    
    # Step 2: Output derivations
    print("Step 2: Generating derivation sequence...")
    derivations = get_productions()
    
    # Read program for context
    program_lines = []
    if os.path.exists(program_path):
        with open(program_path, 'r') as f:
            program_lines = [line.strip() for line in f if line.strip()]
    
    # Write clean formatted output
    derivations_path = os.path.join(LAB_DIR, "derivations.txt")
    
    with open(derivations_path, "w", encoding="utf-8") as f:
        f.write("Derivation Sequence (String of Productions)\n\n")
        f.write("This shows the sequence of grammar productions applied during parsing.\n")
        f.write("Productions are grouped by statement for clarity.\n\n")
        
        # Simple grouping: find each "Stmt ->" and group backwards
        stmt_positions = [i for i, p in enumerate(derivations) if "Stmt ->" in p]
        program_pos = next((i for i, p in enumerate(derivations) if "Program ->" in p), -1)
        
        stmt_num = 1
        processed = set()
        
        for i, stmt_idx in enumerate(stmt_positions):
            if stmt_idx in processed:
                continue
                
            # Get statement type from production before Stmt ->
            stmt_type = "Unknown"
            if stmt_idx > 0:
                prev = derivations[stmt_idx - 1]
                for stmt_name in ["DeclStmt", "OutputStmt", "IfStmt", "AssignStmt", "InputStmt", "WhileStmt", "ExprStmt"]:
                    if f"{stmt_name} ->" in prev:
                        stmt_type = stmt_name
                        break
            
            code = program_lines[stmt_num-1] if stmt_num <= len(program_lines) else ""
            
            # Find start: go back to find statement beginning
            start = stmt_idx
            for j in range(stmt_idx - 1, max(0, stmt_idx - 15) - 1, -1):
                if j in processed:
                    start = j + 1
                    break
                p = derivations[j]
                if any(x in p for x in ["Type ->", "OutputStmt ->", "IfStmt ->", "AssignStmt ->", "InputStmt ->", "WhileStmt ->", "ExprStmt ->"]):
                    start = j
                    break
            
            # Find end: next statement or program
            end = stmt_positions[i+1] if i+1 < len(stmt_positions) else (program_pos if program_pos > stmt_idx else len(derivations))
            
            # Write section
            f.write(f"\nStatement {stmt_num}: {stmt_type}")
            if code:
                f.write(f"  // {code}")
            f.write(f"\n")
            
            # Write productions with indentation
            for idx in range(start, end):
                if idx in processed:
                    continue
                p = derivations[idx]
                if "Factor ->" in p or "Power ->" in p or "Term ->" in p or "Expr ->" in p:
                    indent = "    "
                elif "DeclTail ->" in p or "Cond ->" in p or "CmpOp ->" in p:
                    indent = "      "
                elif "Stmt ->" in p:
                    indent = ""
                else:
                    indent = "  "
                f.write(f"{indent}{idx+1:3d}. {p}\n")
                processed.add(idx)
            
            stmt_num += 1
        
        # Final assembly
        if program_pos >= 0:
            f.write(f"\nFinal Assembly: Building Program\n")
            for idx in range(program_pos, len(derivations)):
                f.write(f"  {idx+1:3d}. {derivations[idx]}\n")
        
        f.write(f"\nTotal: {len(derivations)} productions\n")
        f.write("Parsing method: Bottom-up (LALR) using PLY/yacc\n")
    
    print(f"Derivations written to: {derivations_path}")
    print(f"Total productions: {len(derivations)}")
    print()
    
    # Display first few productions
    if derivations:
        print("First 15 productions:")
        for i, prod in enumerate(derivations[:15], 1):
            print(f"  {i}. {prod}")
        if len(derivations) > 15:
            print(f"  ... and {len(derivations) - 15} more")
    else:
        print("No productions recorded (parsing may have failed)")
    


if __name__ == "__main__":
    # Import lexer module
    import lexer_ply
    main()

