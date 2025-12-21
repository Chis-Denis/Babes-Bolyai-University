%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TABLE_SIZE 211

// Structure for Symbol Table entry
typedef struct Symbol {
    char name[64];
    struct Symbol *next;
} Symbol;

Symbol* hashTable[TABLE_SIZE];

// Hash function (simple polynomial)
int hash(const char *str) {
    int h = 0;
    for (int i = 0; str[i] != '\0'; i++)
        h = (h * 21 + str[i]) % TABLE_SIZE;
    return h;
}

// Insert or find symbol in hash table
int addSymbol(const char *name) {
    int idx = hash(name);
    Symbol *node = hashTable[idx];

    // search if already exists
    while (node != NULL) {
        if (strcmp(node->name, name) == 0)
            return idx; // found
        node = node->next;
    }

    // insert new
    node = (Symbol*) malloc(sizeof(Symbol));
    strcpy(node->name, name);
    node->next = hashTable[idx];
    hashTable[idx] = node;
    return idx;
}

void printSymbolTable(FILE *out) {
    fprintf(out, "===== SYMBOL TABLE =====\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol *node = hashTable[i];
        if (node) {
            fprintf(out, "[%d]: ", i);
            while (node) {
                fprintf(out, "%s ", node->name);
                node = node->next;
            }
            fprintf(out, "\n");
        }
    }
}

// FA for Identifier
int validateIdentifier(const char *str) {
    int state = 0;  // initial state
    int i = 0;
    
    while (str[i] != '\0') {
        char c = str[i];
        if (state == 0) {
            // Must start with a letter
            if (isalpha(c)) {
                state = 1;  // move to accepting state
            } else {
                return 0;  // invalid: doesn't start with letter
            }
        } else if (state == 1) {
            // Can have letters or digits
            if (isalpha(c) || isdigit(c)) {
                state = 1;  // stay in accepting state
            } else {
                return 0;  // invalid character
            }
        }
        i++;
    }
    
    // Must end in accepting state (state 1)
    return (state == 1);
}

// FA for Constant
int validateConstant(const char *str) {
    int state = 0;  // initial state
    int i = 0;
    
    while (str[i] != '\0') {
        char c = str[i];
        if (state == 0) {
            // Must start with a digit
            if (isdigit(c)) {
                state = 1;  // move to accepting state
            } else {
                return 0;  // invalid: doesn't start with digit
            }
        } else if (state == 1) {
            // Can have more digits
            if (isdigit(c)) {
                state = 1;  // stay in accepting state
            } else {
                return 0;  // invalid character
            }
        }
        i++;
    }
    
    // Must end in accepting state (state 1) and have at least one digit
    return (state == 1 && i > 0);
}

FILE *pifFile;
%}

DETOATE    [a-zA-Z0-9]+
OPERATOR    (\+|\-|\*|\/|==|!=|<|>)
KEYWORD     (atribuie|dezvaluie|gogoasa|de_la|pana_la|fa|gata|daca|atunci|altfel|medie)
DELIM       [();=,]

%%

{KEYWORD}   { fprintf(pifFile, "%s - (keyword)\n", yytext); }

{DETOATE}  {
                if (validateConstant(yytext)) {
                    int pos = addSymbol(yytext); 
                    fprintf(pifFile, "CONSTANT(%s) -> bucket %d\n", yytext, pos);
                } else if (validateIdentifier(yytext)) {
                    int pos = addSymbol(yytext); 
                    fprintf(pifFile, "ID(%s) -> bucket %d\n", yytext, pos);
                } else {
                    fprintf(stderr, "Lexical error: nu mere tokenu sefu' '%s'\n", yytext);
                }
            }

{OPERATOR}  { fprintf(pifFile, "%s - (operator)\n", yytext); }
{DELIM}     { fprintf(pifFile, "%s - (delimiter)\n", yytext); }

[ \t\r\n]+  ;

.           { fprintf(stderr, "Lexical error: n-avem simbolu aceste domnu' '%s'\n", yytext); }

%%

int yywrap() { return 1; }

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    FILE *inFile = fopen(argv[1], "r");
    if (!inFile) {
        printf("Error: Cannot open %s\n", argv[1]);
        return 1;
    }

    yyin = inFile;
    pifFile = fopen("PIF.out", "w");

    yylex();

    FILE *stFile = fopen("ST.out", "w");
    printSymbolTable(stFile);

    fclose(pifFile);
    fclose(stFile);
    fclose(inFile);
    return 0;
}