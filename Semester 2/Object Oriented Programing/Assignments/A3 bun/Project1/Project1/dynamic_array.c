#include "dynamic_array.h"
#include <stdlib.h>
#include <stdio.h>

DynamicArray* createDynamicArray(int capacity, DestroyElementFunctionPointer destroyFct)
{
    if (capacity <= 0) capacity = 1;
    DynamicArray* da = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (da == NULL) return NULL;

    da->elements = (TElem*)malloc(sizeof(TElem) * capacity);
    if (da->elements == NULL) {
        free(da);
        return NULL;
    }

    da->length = 0;
    da->capacity = capacity;
    da->destroyElementFunctionPointer = destroyFct;
    return da;
}

void resizeDynamicArray(DynamicArray* dynamicArray)
{
    dynamicArray->capacity *= 2;
    dynamicArray->elements = (TElem*)realloc(dynamicArray->elements, dynamicArray->capacity * sizeof(TElem));
}

int getLengthOfDynamicArray(DynamicArray* dynamicArray)
{
    if (dynamicArray == NULL) return 0;
    return dynamicArray->length;
}

int getCapacityOfDynamicArray(DynamicArray* dynamicArray)
{
    if (dynamicArray == NULL) return 0;
    return dynamicArray->capacity;
}

TElem* getElements(DynamicArray* dynamicArray)
{
    return dynamicArray->elements;
}

DestroyElementFunctionPointer getDestroyElementFunctionPointer(DynamicArray* dynamicArray)
{
    return dynamicArray->destroyElementFunctionPointer;
}

void setLengthOfDynamicArray(DynamicArray* dynamicArray, int length)
{
    dynamicArray->length = length;
}

TElem* getElementOnPosition(DynamicArray* dynamicArray, int position)
{
    if (dynamicArray == NULL || position < 0 || position >= dynamicArray->length) return NULL;
    return dynamicArray->elements[position];
}

void destroyDynamicArray(DynamicArray* dynamicArray)
{
    if (dynamicArray == NULL) return;

    // Destroy all elements
    for (int i = 0; i < dynamicArray->length; i++) {
        if (dynamicArray->elements[i] != NULL && dynamicArray->destroyElementFunctionPointer != NULL) {
            dynamicArray->destroyElementFunctionPointer(dynamicArray->elements[i]);
        }
    }

    // Free the array and the structure
    free(dynamicArray->elements);
    free(dynamicArray);
}

void updateElementOnPosition(DynamicArray* dynamicArray, int position, TElem element)
{
    if (dynamicArray == NULL || position < 0 || position >= dynamicArray->length) return;

    // Destroy old element
    if (dynamicArray->destroyElementFunctionPointer != NULL) {
        dynamicArray->destroyElementFunctionPointer(dynamicArray->elements[position]);
    }

    dynamicArray->elements[position] = element;
}

void swapElementsOnDynamicArray(DynamicArray* dynamicArray, int position1, int position2)
{
    if (dynamicArray == NULL || 
        position1 < 0 || position1 >= dynamicArray->length ||
        position2 < 0 || position2 >= dynamicArray->length) return;

    TElem temp = dynamicArray->elements[position1];
    dynamicArray->elements[position1] = dynamicArray->elements[position2];
    dynamicArray->elements[position2] = temp;
}

void addElementToDynamicArray(DynamicArray* dynamicArray, TElem element)
{
    if (dynamicArray == NULL) return;

    // Resize if necessary
    if (dynamicArray->length == dynamicArray->capacity) {
        int newCapacity = dynamicArray->capacity * 2;
        TElem* newElements = (TElem*)realloc(dynamicArray->elements, sizeof(TElem) * newCapacity);
        if (newElements == NULL) return;

        dynamicArray->elements = newElements;
        dynamicArray->capacity = newCapacity;
    }

    dynamicArray->elements[dynamicArray->length++] = element;
}

void deleteElementFromDynamicArray(DynamicArray* dynamicArray, int position)
{
    if (dynamicArray == NULL || position < 0 || position >= dynamicArray->length) return;

    // Destroy the element at position
    if (dynamicArray->destroyElementFunctionPointer != NULL) {
        dynamicArray->destroyElementFunctionPointer(dynamicArray->elements[position]);
    }

    // Shift elements
    for (int i = position; i < dynamicArray->length - 1; i++) {
        dynamicArray->elements[i] = dynamicArray->elements[i + 1];
    }
    dynamicArray->length--;
}

DynamicArray* copyDynamicArrayOfProducts(DynamicArray* dynamicArray)
{
    if (dynamicArray == NULL) return NULL;

    DynamicArray* copy = createDynamicArray(dynamicArray->capacity, dynamicArray->destroyElementFunctionPointer);
    if (copy == NULL) return NULL;

    for (int i = 0; i < dynamicArray->length; i++) {
        Product* product = (Product*)dynamicArray->elements[i];
        Product* productCopy = copyProduct(product);
        if (productCopy == NULL) {
            destroyDynamicArray(copy);
            return NULL;
        }
        addElementToDynamicArray(copy, productCopy);
    }

    return copy;
}
