#pragma once
#include "product.h"

// Types of operations that can be performed on products
typedef enum {
    OPERATION_ADD,      // Adding a new product or updating quantity
    OPERATION_DELETE,   // Removing a product
    OPERATION_UPDATE    // Modifying product details
} OperationType;

// Stores information about a product operation for undo/redo
typedef struct {
    OperationType type;     // Type of operation performed
    Product before;         // Product state before operation
    Product after;         // Product state after operation
    int is_update;         // Whether this was a quantity update
} Operation;

// Creates a new operation record with before and after states
Operation create_operation(OperationType type, Product before, Product after, int is_update); 