#pragma once
#include "dynamic_array.h"

typedef struct
{
    DynamicArray* dynamicArray;
} Repository;

Repository* createRepository();
int getRepositoryLength(Repository*);
int checkIfProductIsInRepository(Repository*, Product*);
void destroyRepository(Repository*);
void addProductToRepository(Repository*, Product*);
void deleteProductFromRepository(Repository*, int);
void updateProductFromRepository(Repository*, int, Product*);
Product* getProductFromRepository(Repository*, int);
void sortProductBasedOnCondition(Repository*, OperationOfComparisonBetweenProducts);
DynamicArray* getDynamicArray(Repository*);
Repository* copyRepository(Repository*);
void setRepository(Repository*, Repository*);
int isProductExpiringInXDaysRepository(Product*, int);
