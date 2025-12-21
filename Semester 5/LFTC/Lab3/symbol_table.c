#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

#define TABLE_SIZE 97 

Node* table[TABLE_SIZE] = {NULL};

unsigned int hash(const char* s) {
    unsigned int h = 0;
    for (; *s; s++)
        h = (h * 31 + *s) % TABLE_SIZE;
    return h;
}

void insert(const char* s) {
    unsigned int idx = hash(s);
    Node* newNode = malloc(sizeof(Node));
    strcpy(newNode->symbol, s);
    newNode->next = table[idx];
    table[idx] = newNode;
}

int search(const char* s) {
    unsigned int idx = hash(s);
    Node* curr = table[idx];
    while (curr) {
        if (strcmp(curr->symbol, s) == 0)
            return 1;
        curr = curr->next;
    }
    return 0;
}

void display() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* curr = table[i];
        if (curr) {
            printf("[%d] -> ", i);
            while (curr) {
                printf("%s -> ", curr->symbol);
                curr = curr->next;
            }
            printf("NULL\n");
        }
    }
}
