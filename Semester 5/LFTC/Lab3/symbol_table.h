#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct Node {
    char symbol[64];
    struct Node* next;
} Node;

void insert(const char* s);
int search(const char* s);
void display();

#endif
