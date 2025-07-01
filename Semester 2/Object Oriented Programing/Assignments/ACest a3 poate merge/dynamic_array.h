#pragma once
#include "product.h"
#include "operation.h"

// Dynamic array for storing products with automatic resizing
typedef struct {
    Product* items;         // Array of products
    size_t size;           // Current number of items
    size_t capacity;       // Maximum items before resizing
} ProductDynamicArray;

// Dynamic array for storing operation history
typedef struct {
    Operation* items;       // Array of operations
    size_t size;           // Current number of items
    size_t capacity;       // Maximum items before resizing
} OperationDynamicArray;

// Creates a new dynamic array for products with initial capacity
ProductDynamicArray* create_product_array(size_t initial_capacity);

// Frees all memory used by the product array
void destroy_product_array(ProductDynamicArray* arr);

// Adds a product to the array, resizing if necessary
void product_array_add(ProductDynamicArray* arr, Product item);

// Removes a product at the specified index
void product_array_remove_at(ProductDynamicArray* arr, size_t index);

// Finds a product by name and category, returns NULL if not found
Product* product_array_find(ProductDynamicArray* arr, const char* name, Category category);

// Creates a new dynamic array for operations with initial capacity
OperationDynamicArray* create_operation_array(size_t initial_capacity);

// Frees all memory used by the operation array
void destroy_operation_array(OperationDynamicArray* arr);

// Adds an operation to the array, resizing if necessary
void operation_array_add(OperationDynamicArray* arr, Operation item);

// Removes an operation at the specified index
void operation_array_remove_at(OperationDynamicArray* arr, size_t index); 