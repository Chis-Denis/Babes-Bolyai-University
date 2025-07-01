#include "operation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dynamic_array.h"

Operation* createOperation(int typeOfOperation, Product* product1, Product* product2)
{
    Operation* operation = (Operation*)malloc(sizeof(Operation));
    operation->typeOfOperation = typeOfOperation;
    operation->product1 = createProduct(product1->name, product1->category, product1->quantity, product1->expiration_date);
    operation->product2 = NULL;

    if (product2)
        operation->product2 = createProduct(product2->name, product2->category, product2->quantity, product2->expiration_date);

    return operation;
}

Product* getProduct1(Operation* operation)
{
    return operation->product1;
}

Product* getProduct2(Operation* operation)
{
    return operation->product2;
}

int getTypeOfOperation(Operation* operation)
{
    return operation->typeOfOperation;
}

void destroyOperation(TElem element)
{
    Operation* operation = (Operation*)element;
    destroyProduct(operation->product1);
    if (operation->product2)
        destroyProduct(operation->product2);
    free(operation);
}

