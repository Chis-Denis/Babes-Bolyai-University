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
void resizeDynamicArray(DynamicArray*);
void destroyDynamicArray(DynamicArray*);
void setDynamicArray(DynamicArray*, DynamicArray*);
void setLengthOfDynamicArray(DynamicArray*, int);
TElem* getElementOnPosition(DynamicArray*, int);
TElem* getElements(DynamicArray*);
void addElementToDynamicArray(DynamicArray*, TElem);
void deleteElementFromDynamicArray(DynamicArray*, int);
void updateElementOnPosition(DynamicArray*, int, TElem);
void swapElementsOnDynamicArray(DynamicArray*, int, int);
int getLengthOfDynamicArray(DynamicArray*);
int getCapacityOfDynamicArray(DynamicArray*);
DestroyElementFunctionPointer getDestroyElementFunctionPointer(DynamicArray*);
DynamicArray* copyDynamicArrayOfProducts(DynamicArray*);