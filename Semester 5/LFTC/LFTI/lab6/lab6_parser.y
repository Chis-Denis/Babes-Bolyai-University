%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;
extern FILE *yyin;
extern int yylex(void);

typedef struct Production {
    char rule[256];
    struct Production *next;
} Production;

Production *productions = NULL;
int production_count = 0;

void log_production(const char *rule) {
    Production *p = (Production*)malloc(sizeof(Production));
    strcpy(p->rule, rule);
    p->next = productions;
    productions = p;
    production_count++;
}

void print_productions(FILE *out) {
    Production *reversed = NULL;
    Production *current = productions;
    
    while (current != NULL) {
        Production *next = current->next;
        current->next = reversed;
        reversed = current;
        current = next;
    }
    
    fprintf(out, "Derivation Sequence\n");
    fprintf(out, "==========================================\n\n");
    
    int num = 1;
    current = reversed;
    while (current != NULL) {
        fprintf(out, "%3d. %s\n", num++, current->rule);
        current = current->next;
    }
    
    fprintf(out, "\nTotal: %d productions\n", production_count);
}

void free_productions() {
    Production *current = productions;
    while (current != NULL) {
        Production *next = current->next;
        free(current);
        current = next;
    }
    productions = NULL;
    production_count = 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "Avem eroare aici %d: %s\n", yylineno, s);
    return 0;
}
%}

%token ATRIBUIE DEZVALUIE GOGOASA DACA ATUNCI GATA DE_LA PANA_LA FA MEDIE ALTEL
%token PLUS MINUS MULT DIV EQ NE GT LT ASSIGN
%token OPEN_PAREN CLOSE_PAREN COMMA SEMICOLON
%token ID NUMBER

%left PLUS MINUS
%left MULT DIV
%nonassoc GT LT EQ NE

%%

program:
    stmt_list {
        log_production("Program -> StmtList");
        $$ = 1;
    }
    ;

stmt_list:
    stmt {
        log_production("StmtList -> Stmt");
        $$ = 1;
    }
    | stmt_list stmt {
        log_production("StmtList -> StmtList Stmt");
        $$ = 1;
    }
    ;

stmt:
    assign_stmt {
        log_production("Stmt -> AssignStmt");
        $$ = 1;
    }
    | print_stmt {
        log_production("Stmt -> PrintStmt");
        $$ = 1;
    }
    | for_stmt {
        log_production("Stmt -> ForStmt");
        $$ = 1;
    }
    | if_stmt {
        log_production("Stmt -> IfStmt");
        $$ = 1;
    }
    | mean_stmt {
        log_production("Stmt -> MeanStmt");
        $$ = 1;
    }
    ;

assign_stmt:
    ATRIBUIE ID ASSIGN expr SEMICOLON {
        log_production("AssignStmt -> ATRIBUIE ID ASSIGN Expr SEMICOLON");
        $$ = 1;
    }
    ;

print_stmt:
    DEZVALUIE OPEN_PAREN ID CLOSE_PAREN SEMICOLON {
        log_production("PrintStmt -> DEZVALUIE OPEN_PAREN ID CLOSE_PAREN SEMICOLON");
        $$ = 1;
    }
    ;

for_stmt:
    GOGOASA ID DE_LA NUMBER PANA_LA NUMBER FA stmt_list GATA {
        log_production("ForStmt -> GOGOASA ID DE_LA NUMBER PANA_LA NUMBER FA StmtList GATA");
        $$ = 1;
    }
    ;

if_stmt:
    DACA OPEN_PAREN cond CLOSE_PAREN ATUNCI stmt_list GATA {
        log_production("IfStmt -> DACA OPEN_PAREN Cond CLOSE_PAREN ATUNCI StmtList GATA");
        $$ = 1;
    }
    | DACA OPEN_PAREN cond CLOSE_PAREN ATUNCI stmt_list ALTEL stmt_list GATA {
        log_production("IfStmt -> DACA OPEN_PAREN Cond CLOSE_PAREN ATUNCI StmtList ALTEL StmtList GATA");
        $$ = 1;
    }
    ;

mean_stmt:
    MEDIE OPEN_PAREN id_list CLOSE_PAREN SEMICOLON {
        log_production("MeanStmt -> MEDIE OPEN_PAREN IdList CLOSE_PAREN SEMICOLON");
        $$ = 1;
    }
    ;

id_list:
    ID {
        log_production("IdList -> ID");
        $$ = 1;
    }
    | id_list COMMA ID {
        log_production("IdList -> IdList COMMA ID");
        $$ = 1;
    }
    ;

cond:
    expr cmp_op expr {
        log_production("Cond -> Expr CmpOp Expr");
        $$ = 1;
    }
    ;

cmp_op:
    GT {
        log_production("CmpOp -> GT");
        $$ = 1;
    }
    | LT {
        log_production("CmpOp -> LT");
        $$ = 1;
    }
    | EQ {
        log_production("CmpOp -> EQ");
        $$ = 1;
    }
    | NE {
        log_production("CmpOp -> NE");
        $$ = 1;
    }
    ;

expr:
    term {
        log_production("Expr -> Term");
        $$ = 1;
    }
    | expr PLUS term {
        log_production("Expr -> Expr PLUS Term");
        $$ = 1;
    }
    | expr MINUS term {
        log_production("Expr -> Expr MINUS Term");
        $$ = 1;
    }
    ;

term:
    factor {
        log_production("Term -> Factor");
        $$ = 1;
    }
    | term MULT factor {
        log_production("Term -> Term MULT Factor");
        $$ = 1;
    }
    | term DIV factor {
        log_production("Term -> Term DIV Factor");
        $$ = 1;
    }
    ;

factor:
    NUMBER {
        log_production("Factor -> NUMBER");
        $$ = 1;
    }
    | ID {
        log_production("Factor -> ID");
        $$ = 1;
    }
    | OPEN_PAREN expr CLOSE_PAREN {
        log_production("Factor -> OPEN_PAREN Expr CLOSE_PAREN");
        $$ = 1;
    }
    ;

%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    FILE *inFile = fopen(argv[1], "r");
    if (!inFile) {
        fprintf(stderr, "Error: Nu pot deschide boss %s\n", argv[1]);
        return 1;
    }

    yyin = inFile;
    
    int result = yyparse();
    
    fclose(inFile);

    if (result == 0) {
        printf("Parsing successful!\n");
        
        char outfile[256];
        strcpy(outfile, argv[1]);
        char *ext = strrchr(outfile, '.');
        if (ext) {
            *ext = '\0';
        }
        strcat(outfile, "_derivations.txt");
        
        FILE *out = fopen(outfile, "w");
        if (out) {
            print_productions(out);
            fclose(out);
            printf("Derivations written to: %s\n", outfile);
        } else {
            fprintf(stderr, "Error: Cannot write to %s\n", outfile);
        }
    } else {
        fprintf(stderr, "Parsing failed!\n");
    }

    free_productions();
    return result;
}

