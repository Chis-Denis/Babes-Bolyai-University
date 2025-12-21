"""
Convert parse tree to table format with father/sibling relations
"""

from typing import Optional, Dict
from ll1_parser import ParseTreeNode


def build_tree_table(root: Optional[ParseTreeNode]) -> List[Dict]:
    """
    Build table representation of parse tree.
    Returns list of dicts with: node_id, symbol, father_id, sibling_id
    """
    if not root:
        return []
    
    table = []
    node_map = {}  # node_id -> table index
    
    def traverse(node: ParseTreeNode, father_id: int = 0):
        """Traverse tree and build table"""
        # Add this node to table
        table_index = len(table)
        node_map[node.node_id] = table_index
        
        # Find sibling ID
        sibling_id = 0
        if node.sibling:
            sibling_id = node.sibling.node_id
        
        # Add to table
        entry = {
            'node_id': node.node_id,
            'symbol': node.symbol,
            'father_id': father_id,
            'sibling_id': sibling_id if sibling_id > 0 else 0,
            'production': node.production.index if node.production else None
        }
        table.append(entry)
        
        # Traverse children
        for child in node.children:
            traverse(child, node.node_id)
    
    traverse(root)
    return table


def print_tree_table(table: List[Dict], output_file: str = None):
    """Print parse tree table in readable format"""
    lines = []
    lines.append("Parse Tree Table (Father/Sibling Relations)")
    lines.append("=" * 70)
    lines.append("")
    lines.append(f"{'Node':<8} {'Symbol':<25} {'Father':<8} {'Sibling':<8} {'Production':<10}")
    lines.append("-" * 70)
    
    # Sort by node_id for readability
    sorted_table = sorted(table, key=lambda x: x['node_id'])
    
    for entry in sorted_table:
        node_id = entry['node_id']
        symbol = entry['symbol']
        father = entry['father_id'] if entry['father_id'] > 0 else '0'
        sibling = entry['sibling_id'] if entry['sibling_id'] > 0 else '0'
        prod = str(entry['production']) if entry['production'] is not None else '-'
        
        # Replace epsilon with 'epsilon' for readability
        symbol = symbol.replace('ε', 'epsilon')
        
        # Truncate long symbols
        if len(symbol) > 24:
            symbol = symbol[:21] + "..."
        
        lines.append(f"{node_id:<8} {symbol:<25} {father:<8} {sibling:<8} {prod:<10}")
    
    lines.append("")
    lines.append("=" * 70)
    lines.append(f"Total nodes: {len(table)}")
    lines.append("")
    
    output = "\n".join(lines)
    
    if output_file:
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(output)
    
    print(output)
    return output


def get_productions_string(root: Optional[ParseTreeNode]) -> List[str]:
    """Get string of productions from parse tree"""
    productions = []
    
    def traverse(node: ParseTreeNode):
        if node.production:
            rhs = ' '.join(node.production.right) if node.production.right else 'epsilon'
            productions.append(f"{node.production.left} -> {rhs}")
        
        for child in node.children:
            traverse(child)
    
    if root:
        traverse(root)
    
    return productions

