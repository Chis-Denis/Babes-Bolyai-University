#pragma once
#include "product.h"

typedef struct
{
    /*
    ** typeOfOperation: 1 - Add, 2 - Delete, 3 - Update
    ** 1) Add product1, product2 = NULL
    ** 2) Delete product1, product2 = NULL
    ** 3) Update product1, product2 (before and after update)
    */
    int typeOfOperation;
    Product* product1;
    Product* product2; // Used for update operations (before and after update)
} Operation;

Operation* createOperation(int typeOfOperation, Product* product1, Product* product2);
void destroyOperation(Operation* operation);
int getTypeOfOperation(Operation* operation);
Product* getProduct1(Operation* operation);
Product* getProduct2(Operation* operation);
