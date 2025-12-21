#include <stdio.h>
#include "symbol_table.h"

int main() {
    insert("a");
    insert("b");
    insert("GCD");
    insert("OUTPUT");
    insert("10");
    insert("5");

    printf("Symbol Table Contents:\n");
    display();

    printf("\nSearching for symbols:\n");
    printf("Found a? %d\n", search("a"));
    printf("Found z? %d\n", search("z"));

    return 0;
}
