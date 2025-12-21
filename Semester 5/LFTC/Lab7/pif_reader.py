"""
Read PIF and ST files from Lab 4 and convert to token stream
"""

import os
from typing import List, Tuple


def read_pif_and_st(pif_path: str, st_path: str) -> List[str]:
    """
    Read PIF and ST files and convert to list of token strings.
    Returns list like: ['int', 'id', '=', 'const', ...]
    """
    # Read ST to map indices to actual values
    st_map = {'identifiers': {}, 'constants': {}}
    
    with open(st_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    current_section = None
    for line in lines:
        line = line.strip()
        if line.startswith('Identifiers:'):
            current_section = 'identifiers'
            continue
        elif line.startswith('Constants:'):
            current_section = 'constants'
            continue
        
        if '->' in line and current_section:
            parts = line.split('->', 1)
            idx = int(parts[0].strip())
            value = parts[1].strip()
            st_map[current_section][idx] = value
    
    # Read PIF
    tokens = []
    with open(pif_path, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            
            # Parse: ('token_type', index) - Python tuple format
            try:
                # Use eval to parse the tuple (safe in this context)
                token_tuple = eval(line)
                if isinstance(token_tuple, tuple) and len(token_tuple) >= 2:
                    token_type = str(token_tuple[0])
                    idx = token_tuple[1]
                    
                    if idx == -1:
                        # Reserved word or operator - use as-is
                        tokens.append(token_type)
                    else:
                        # Identifier or constant - use 'id' or 'const' as terminal
                        if token_type == 'id':
                            tokens.append('id')
                        elif token_type == 'const':
                            tokens.append('const')
                        else:
                            tokens.append(token_type)
            except:
                # Fallback: manual parsing
                line = line.strip("()")
                parts = [p.strip().strip("'\"") for p in line.split(',')]
                
                if len(parts) >= 2:
                    token_type = parts[0]
                    idx_str = parts[1].strip()
                    
                    if idx_str == '-1':
                        tokens.append(token_type)
                    else:
                        if token_type == 'id':
                            tokens.append('id')
                        elif token_type == 'const':
                            tokens.append('const')
                        else:
                            tokens.append(token_type)
    
    return tokens

