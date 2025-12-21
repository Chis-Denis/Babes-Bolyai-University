"""
PLY Parser for Lab 6 with production tracking
Outputs derivation sequence (string of productions)
"""

import ply.yacc as yacc
from lexer_ply import tokens, lexer

# Global list to track productions
productions = []

def log_production(rule_name, children=None):
    """Log a production rule application"""
    if children:
        rhs = ' '.join([str(c) if c else 'ε' for c in children])
        productions.append(f"{rule_name} -> {rhs}")
    else:
        productions.append(f"{rule_name}")

# Precedence rules (lowest to highest)
precedence = (
    ('left', 'PLUS', 'MINUS'),
    ('left', 'MULT', 'DIV'),
    ('right', 'POW', 'LCM', 'POWER_SUM'),
)

# Grammar rules
def p_program(p):
    """Program : StmtList"""
    log_production('Program', ['StmtList'])
    p[0] = ('Program', p[1])

def p_stmt_list(p):
    """StmtList : Stmt StmtList
                | empty"""
    if len(p) == 3:
        log_production('StmtList', ['Stmt', 'StmtList'])
        p[0] = ('StmtList', p[1], p[2])
    else:
        log_production('StmtList', ['ε'])
        p[0] = ('StmtList',)

def p_stmt(p):
    """Stmt : DeclStmt
            | AssignStmt
            | InputStmt
            | OutputStmt
            | IfStmt
            | WhileStmt
            | ExprStmt"""
    if p[1][0] == 'DeclStmt':
        log_production('Stmt', ['DeclStmt'])
    elif p[1][0] == 'AssignStmt':
        log_production('Stmt', ['AssignStmt'])
    elif p[1][0] == 'InputStmt':
        log_production('Stmt', ['InputStmt'])
    elif p[1][0] == 'OutputStmt':
        log_production('Stmt', ['OutputStmt'])
    elif p[1][0] == 'IfStmt':
        log_production('Stmt', ['IfStmt'])
    elif p[1][0] == 'WhileStmt':
        log_production('Stmt', ['WhileStmt'])
    elif p[1][0] == 'ExprStmt':
        log_production('Stmt', ['ExprStmt'])
    p[0] = ('Stmt', p[1])

def p_decl_stmt(p):
    """DeclStmt : Type ID DeclTail"""
    log_production('DeclStmt', ['Type', 'ID', 'DeclTail'])
    p[0] = ('DeclStmt', p[1], p[2], p[3])

def p_decl_tail_assign(p):
    """DeclTail : ASSIGN Expr"""
    log_production('DeclTail', ['ASSIGN', 'Expr'])
    p[0] = ('DeclTail', p[2])

def p_decl_tail_empty(p):
    """DeclTail : empty"""
    log_production('DeclTail', ['ε'])
    p[0] = ('DeclTail',)

def p_type(p):
    """Type : INT
            | STR
            | CHAR
            | ARRAY_INT
            | ARRAY_STR"""
    log_production('Type', [p[1]])
    p[0] = ('Type', p[1])

def p_assign_stmt(p):
    """AssignStmt : ID ASSIGN Expr"""
    log_production('AssignStmt', ['ID', 'ASSIGN', 'Expr'])
    p[0] = ('AssignStmt', p[1], p[3])

def p_input_stmt(p):
    """InputStmt : INPUT ID"""
    log_production('InputStmt', ['INPUT', 'ID'])
    p[0] = ('InputStmt', p[2])

def p_output_stmt(p):
    """OutputStmt : OUTPUT Expr"""
    log_production('OutputStmt', ['OUTPUT', 'Expr'])
    p[0] = ('OutputStmt', p[2])

def p_if_stmt(p):
    """IfStmt : IF Cond THEN StmtList ElsePart END"""
    log_production('IfStmt', ['IF', 'Cond', 'THEN', 'StmtList', 'ElsePart', 'END'])
    p[0] = ('IfStmt', p[2], p[4], p[5])

def p_else_part(p):
    """ElsePart : ELSE StmtList
                | empty"""
    if len(p) == 3:
        log_production('ElsePart', ['ELSE', 'StmtList'])
        p[0] = ('ElsePart', p[2])
    else:
        log_production('ElsePart', ['ε'])
        p[0] = ('ElsePart',)

def p_while_stmt(p):
    """WhileStmt : WHILE Cond DO StmtList END"""
    log_production('WhileStmt', ['WHILE', 'Cond', 'DO', 'StmtList', 'END'])
    p[0] = ('WhileStmt', p[2], p[4])

def p_expr_stmt(p):
    """ExprStmt : Expr"""
    log_production('ExprStmt', ['Expr'])
    p[0] = ('ExprStmt', p[1])

def p_expr(p):
    """Expr : Term
            | Expr PLUS Term
            | Expr MINUS Term"""
    if len(p) == 2:
        log_production('Expr', ['Term'])
        p[0] = ('Expr', p[1])
    elif p[2] == 'PLUS':
        log_production('Expr', ['Expr', 'PLUS', 'Term'])
        p[0] = ('Expr', p[1], '+', p[3])
    else:
        log_production('Expr', ['Expr', 'MINUS', 'Term'])
        p[0] = ('Expr', p[1], '-', p[3])

def p_term(p):
    """Term : Power
            | Term MULT Power
            | Term DIV Power"""
    if len(p) == 2:
        log_production('Term', ['Power'])
        p[0] = ('Term', p[1])
    elif p[2] == 'MULT':
        log_production('Term', ['Term', 'MULT', 'Power'])
        p[0] = ('Term', p[1], '*', p[3])
    else:
        log_production('Term', ['Term', 'DIV', 'Power'])
        p[0] = ('Term', p[1], '/', p[3])

def p_power(p):
    """Power : Factor
             | Power POW Factor
             | Power LCM Factor
             | Power POWER_SUM Factor"""
    if len(p) == 2:
        log_production('Power', ['Factor'])
        p[0] = ('Power', p[1])
    elif p[2] == 'POW':
        log_production('Power', ['Power', 'POW', 'Factor'])
        p[0] = ('Power', p[1], '^^', p[3])
    elif p[2] == 'LCM':
        log_production('Power', ['Power', 'LCM', 'Factor'])
        p[0] = ('Power', p[1], '%%', p[3])
    else:
        log_production('Power', ['Power', 'POWER_SUM', 'Factor'])
        p[0] = ('Power', p[1], '@@', p[3])

def p_factor_id(p):
    """Factor : ID"""
    log_production('Factor', ['ID'])
    p[0] = ('Factor', p[1])

def p_factor_const(p):
    """Factor : CONST"""
    log_production('Factor', ['CONST'])
    p[0] = ('Factor', p[1])

def p_factor_paren(p):
    """Factor : LPAREN Expr RPAREN"""
    log_production('Factor', ['LPAREN', 'Expr', 'RPAREN'])
    p[0] = ('Factor', p[2])

def p_factor_array(p):
    """Factor : ArrayAccess"""
    log_production('Factor', ['ArrayAccess'])
    p[0] = ('Factor', p[1])

def p_factor_func(p):
    """Factor : FuncCall"""
    log_production('Factor', ['FuncCall'])
    p[0] = ('Factor', p[1])

def p_array_access(p):
    """ArrayAccess : ID LBRACKET Expr RBRACKET"""
    log_production('ArrayAccess', ['ID', 'LBRACKET', 'Expr', 'RBRACKET'])
    p[0] = ('ArrayAccess', p[1], p[3])

def p_func_call(p):
    """FuncCall : ID LPAREN ArgListOpt RPAREN"""
    log_production('FuncCall', ['ID', 'LPAREN', 'ArgListOpt', 'RPAREN'])
    p[0] = ('FuncCall', p[1], p[3])

def p_arg_list_opt(p):
    """ArgListOpt : ArgList
                  | empty"""
    if len(p) == 2:
        log_production('ArgListOpt', ['ArgList'])
        p[0] = ('ArgListOpt', p[1])
    else:
        log_production('ArgListOpt', ['ε'])
        p[0] = ('ArgListOpt',)

def p_arg_list(p):
    """ArgList : Expr
               | ArgList COMMA Expr"""
    if len(p) == 2:
        log_production('ArgList', ['Expr'])
        p[0] = ('ArgList', [p[1]])
    else:
        log_production('ArgList', ['ArgList', 'COMMA', 'Expr'])
        p[0] = ('ArgList', p[1] + [p[3]])

def p_cond(p):
    """Cond : Expr CmpOp Expr"""
    log_production('Cond', ['Expr', 'CmpOp', 'Expr'])
    p[0] = ('Cond', p[1], p[2], p[3])

def p_cmp_op(p):
    """CmpOp : EQ
             | NE
             | GT
             | LT
             | GE
             | LE"""
    log_production('CmpOp', [p[1]])
    p[0] = ('CmpOp', p[1])

def p_empty(p):
    """empty :"""
    pass

def p_error(p):
    if p:
        print(f"Syntax error at token '{p.value}' (type: {p.type}) at line {p.lineno}")
    else:
        print("Syntax error at EOF")

# Build parser
parser = yacc.yacc(debug=False)

def get_productions():
    """Get the list of productions applied during parsing"""
    return productions.copy()

def reset_productions():
    """Reset the productions list"""
    global productions
    productions = []

