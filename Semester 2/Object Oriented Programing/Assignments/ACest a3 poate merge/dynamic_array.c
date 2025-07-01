#include "dynamic_array.h"
#include <stdlib.h>
#include <string.h>

// Product array implementation
ProductDynamicArray* create_product_array(size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    
    ProductDynamicArray* arr = (ProductDynamicArray*)malloc(sizeof(ProductDynamicArray));
    if (arr == NULL) return NULL;
    
    arr->items = (Product*)malloc(initial_capacity * sizeof(Product));
    if (arr->items == NULL) {
        free(arr);
        return NULL;
    }
    
    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

void destroy_product_array(ProductDynamicArray* arr) {
    if (arr != NULL) {
        free(arr->items);
        free(arr);
    }
}

static int ensure_product_array_capacity(ProductDynamicArray* arr) {
    if (arr == NULL) return 0;
    
    if (arr->size >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        Product* new_items = (Product*)realloc(arr->items, new_capacity * sizeof(Product));
        if (new_items == NULL) return 0;
        
        arr->items = new_items;
        arr->capacity = new_capacity;
    }
    return 1;
}

void product_array_add(ProductDynamicArray* arr, Product item) {
    if (arr == NULL) return;
    
    if (ensure_product_array_capacity(arr)) {
        arr->items[arr->size++] = item;
    }
}

void product_array_remove_at(ProductDynamicArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) return;
    
    for (size_t i = index; i < arr->size - 1; i++) {
        arr->items[i] = arr->items[i + 1];
    }
    arr->size--;
}

Product* product_array_find(ProductDynamicArray* arr, const char* name, Category category) {
    if (arr == NULL || name == NULL) return NULL;
    
    for (size_t i = 0; i < arr->size; i++) {
        if (strcmp(arr->items[i].name, name) == 0 && arr->items[i].category == category) {
            return &arr->items[i];
        }
    }
    return NULL;
}

// Operation array implementation
OperationDynamicArray* create_operation_array(size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    
    OperationDynamicArray* arr = (OperationDynamicArray*)malloc(sizeof(OperationDynamicArray));
    if (arr == NULL) return NULL;
    
    arr->items = (Operation*)malloc(initial_capacity * sizeof(Operation));
    if (arr->items == NULL) {
        free(arr);
        return NULL;
    }
    
    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

void destroy_operation_array(OperationDynamicArray* arr) {
    if (arr != NULL) {
        free(arr->items);
        free(arr);
    }
}

static int ensure_operation_array_capacity(OperationDynamicArray* arr) {
    if (arr == NULL) return 0;
    
    if (arr->size >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        Operation* new_items = (Operation*)realloc(arr->items, new_capacity * sizeof(Operation));
        if (new_items == NULL) return 0;
        
        arr->items = new_items;
        arr->capacity = new_capacity;
    }
    return 1;
}

//
void operation_array_add(OperationDynamicArray* arr, Operation item) {
    if (arr == NULL) return;
    
    if (ensure_operation_array_capacity(arr)) {
        arr->items[arr->size++] = item;
    }
}

void operation_array_remove_at(OperationDynamicArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) return;
    
    for (size_t i = index; i < arr->size - 1; i++) {
        arr->items[i] = arr->items[i + 1];
    }
    arr->size--;
} 