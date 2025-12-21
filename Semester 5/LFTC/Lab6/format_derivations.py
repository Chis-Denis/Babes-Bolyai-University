"""
Format derivations.txt in a more readable way
"""

def format_derivations_file(derivations_path, program_path=None):
    """Read and reformat the derivations file"""
    
    # Read current derivations
    with open(derivations_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    # Extract productions (skip header)
    productions = []
    for line in lines:
        line = line.strip()
        if line and line[0].isdigit():
            # Extract production (everything after number and dot)
            parts = line.split('.', 1)
            if len(parts) == 2:
                num = parts[0].strip()
                prod = parts[1].strip()
                productions.append((int(num), prod))
    
    # Read program for context
    program_lines = []
    if program_path:
        try:
            with open(program_path, 'r') as f:
                program_lines = [line.strip() for line in f if line.strip()]
        except:
            pass
    
    # Group by statements
    output = []
    output.append("Derivation Sequence (String of Productions)")
    output.append("=" * 70)
    output.append("")
    output.append("This shows the sequence of grammar productions applied during parsing.")
    output.append("Productions are grouped by the program construct they parse.")
    output.append("")
    output.append("=" * 70)
    output.append("")
    
    # Find statement boundaries
    stmt_boundaries = []
    for num, prod in productions:
        if "Stmt ->" in prod:
            stmt_boundaries.append(num)
    
    # Group productions
    stmt_num = 1
    i = 0
    
    while i < len(productions):
        num, prod = productions[i]
        
        # Check if this is a statement start
        if "Stmt ->" in prod:
            # Find all productions for this statement (go back to find start)
            start_idx = i
            while start_idx > 0 and "Stmt ->" not in productions[start_idx-1][1]:
                start_idx -= 1
                # Don't go back too far
                if i - start_idx > 20:
                    start_idx = i - 15
                    break
            
            # Find end (next Stmt or Program)
            end_idx = len(productions)
            for j in range(i+1, len(productions)):
                if "Stmt ->" in productions[j][1] or "Program ->" in productions[j][1]:
                    end_idx = j
                    break
            
            # Get statement type
            stmt_type = prod.split("->")[1].strip()
            
            # Get program context
            context = ""
            if program_lines and stmt_num <= len(program_lines):
                context = f"  // {program_lines[stmt_num-1]}"
            
            # Write section
            output.append("")
            output.append("─" * 70)
            output.append(f"STATEMENT {stmt_num}: {stmt_type}{context}")
            output.append("─" * 70)
            
            # Write productions
            for j in range(start_idx, end_idx):
                pnum, pprod = productions[j]
                
                # Indentation
                if "Factor ->" in pprod or "Power ->" in pprod or "Term ->" in pprod or "Expr ->" in pprod:
                    indent = "    "
                elif "DeclTail ->" in pprod or "Cond ->" in pprod or "CmpOp ->" in pprod or "ElsePart ->" in pprod:
                    indent = "      "
                elif "Type ->" in pprod or "OutputStmt ->" in pprod or "DeclStmt ->" in pprod:
                    indent = "  "
                elif "Stmt ->" in pprod:
                    indent = ""
                else:
                    indent = "  "
                
                output.append(f"{indent}{pnum:3d}. {pprod}")
            
            stmt_num += 1
            i = end_idx
            continue
        
        # Check for Program
        if "Program ->" in prod:
            output.append("")
            output.append("═" * 70)
            output.append("FINAL ASSEMBLY: Building Complete Program")
            output.append("═" * 70)
            
            # Write remaining
            for j in range(i, len(productions)):
                pnum, pprod = productions[j]
                output.append(f"  {pnum:3d}. {pprod}")
            break
        
        i += 1
    
    # Summary
    output.append("")
    output.append("═" * 70)
    output.append("SUMMARY")
    output.append("═" * 70)
    output.append(f"Total productions applied: {len(productions)}")
    output.append("Parsing method: Bottom-up (LALR) using PLY/yacc")
    output.append("Grammar: Context-Free Grammar for mini DSL")
    output.append("")
    
    # Write back
    with open(derivations_path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(output))
    
    print(f"Formatted {derivations_path}")

if __name__ == "__main__":
    import os
    lab_dir = os.path.dirname(__file__)
    lab4_dir = os.path.join(lab_dir, "..", "Lab4")
    format_derivations_file(
        os.path.join(lab_dir, "derivations.txt"),
        os.path.join(lab4_dir, "program.txt")
    )
