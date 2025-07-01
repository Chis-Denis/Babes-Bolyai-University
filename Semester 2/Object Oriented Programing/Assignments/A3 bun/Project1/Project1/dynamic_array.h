#pragma once
#include "product.h"

typedef void* TElem;
typedef void (*DestroyElementFunctionPointer)(TElem);

typedef struct
{
    TElem* elements;
    int length, capacity;
    DestroyElementFunctionPointer destroyElementFunctionPointer;
} DynamicArray;

DynamicArray* createDynamicArray(int, DestroyElementFunctionPointer);
void destroyDynamicArray(DynamicArray*);
void setLengthOfDynamicArray(DynamicArray*, int);
TElem* getElementOnPosition(DynamicArray*, int);
void addElementToDynamicArray(DynamicArray*, TElem);
void deleteElementFromDynamicArray(DynamicArray*, int);
void updateElementOnPosition(DynamicArray*, int, TElem);
void swapElementsOnDynamicArray(DynamicArray*, int, int);
int getLengthOfDynamicArray(DynamicArray*);
int getCapacityOfDynamicArray(DynamicArray*);
DestroyElementFunctionPointer getDestroyElementFunctionPointer(DynamicArray*);
DynamicArray* copyDynamicArrayOfProducts(DynamicArray*);
