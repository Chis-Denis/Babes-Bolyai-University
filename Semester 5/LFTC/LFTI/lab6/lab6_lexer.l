%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lab6_parser.tab.h"

char id_buffer[256];

int validateIdentifier(const char *str) {
    int state = 0;
    int i = 0;
    
    while (str[i] != '\0') {
        char c = str[i];
        if (state == 0) {
            if (isalpha(c)) {
                state = 1;
            } else {
                return 0;
            }
        } else if (state == 1) {
            if (isalpha(c) || isdigit(c)) {
                state = 1;
            } else {
                return 0;
            }
        }
        i++;
    }
    
    return (state == 1);
}

int validateConstant(const char *str) {
    int state = 0;
    int i = 0;
    
    while (str[i] != '\0') {
        char c = str[i];
        if (state == 0) {
            if (isdigit(c)) {
                state = 1;
            } else {
                return 0;
            }
        } else if (state == 1) {
            if (isdigit(c)) {
                state = 1;
            } else {
                return 0;
            }
        }
        i++;
    }
    
    return (state == 1 && i > 0);
}
%}

%option noyywrap
%option yylineno

DETOATE    [a-zA-Z0-9]+
OPERATOR    (\+|\-|\*|\/|==|!=|<|>)
KEYWORD     (atribuie|dezvaluie|gogoasa|de_la|pana_la|fa|gata|daca|atunci|altfel|medie)
DELIM       [();=,]

%%

{KEYWORD}   {
                if (strcmp(yytext, "atribuie") == 0) return ATRIBUIE;
                if (strcmp(yytext, "dezvaluie") == 0) return DEZVALUIE;
                if (strcmp(yytext, "gogoasa") == 0) return GOGOASA;
                if (strcmp(yytext, "de_la") == 0) return DE_LA;
                if (strcmp(yytext, "pana_la") == 0) return PANA_LA;
                if (strcmp(yytext, "fa") == 0) return FA;
                if (strcmp(yytext, "gata") == 0) return GATA;
                if (strcmp(yytext, "daca") == 0) return DACA;
                if (strcmp(yytext, "atunci") == 0) return ATUNCI;
                if (strcmp(yytext, "altfel") == 0) return ALTEL;
                if (strcmp(yytext, "medie") == 0) return MEDIE;
                return 0;
            }

{DETOATE}  {
                if (validateConstant(yytext)) {
                    yylval = atoi(yytext);
                    return NUMBER;
                } else if (validateIdentifier(yytext)) {
                    strncpy(id_buffer, yytext, 255);
                    id_buffer[255] = '\0';
                    return ID;
                } else {
                    fprintf(stderr, "Lexical error: invalid token '%s'\n", yytext);
                    return 0;
                }
            }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"=="        { return EQ; }
"!="        { return NE; }
">"         { return GT; }
"<"         { return LT; }
"="         { return ASSIGN; }
"("         { return OPEN_PAREN; }
")"         { return CLOSE_PAREN; }
","         { return COMMA; }
";"         { return SEMICOLON; }

[ \t\r\n]+  { /* ignore whitespace */ }

.           { 
                fprintf(stderr, "Lexical error: Nu stiu sefu ce-i cu caracteru asta '%s'\n", yytext);
                return 0;
            }

%%

