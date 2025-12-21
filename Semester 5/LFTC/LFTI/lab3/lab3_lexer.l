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

FILE *pifFile;
%}

DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT})*
NUMBER      {DIGIT}+
OPERATOR    (\+|\-|\*|\/|==|!=|<|>)
KEYWORD     (atribuie|dezvaluie|gogoasa|de_la|pana_la|fa|gata|daca|atunci|altfel|medie)
DELIM       [();=,]

%%

{KEYWORD}   { fprintf(pifFile, "%s - (keyword)\n", yytext); }

{ID}        { 
                int pos = addSymbol(yytext); 
                fprintf(pifFile, "ID(%s) -> bucket %d\n", yytext, pos);
            }

{NUMBER}    { 
                int pos = addSymbol(yytext); 
                fprintf(pifFile, "CONST(%s) -> bucket %d\n", yytext, pos);
            }

{OPERATOR}  { fprintf(pifFile, "%s - (operator)\n", yytext); }
{DELIM}     { fprintf(pifFile, "%s - (delimiter)\n", yytext); }

[ \t\r\n]+  ;   // ignore spatiu

.           { fprintf(stderr, "Lexical error: unknown symbol '%s'\n", yytext); }

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
