#pragma once
#include "product.h"
#include "dynamic_array.h"
#include "operation.h"

// Special value used to search across all categories
#define CATEGORY_ANY ((Category)-1)

// Main storage structure for products and operation history
typedef struct {
    ProductDynamicArray* products;
    OperationDynamicArray* undo_stack;
    OperationDynamicArray* redo_stack;
} Repository;

// Creates a new empty repository with initialized storage
Repository* create_repository();

// Frees all memory associated with the repository
void destroy_repository(Repository* repo);

// Adds a new product or updates quantity if it exists
void repository_add_product(Repository* repo, Product product);

// Removes a product from the repository by name and category
void repository_delete_product(Repository* repo, const char* name, Category category);

// Updates all fields of an existing product
void repository_update_product(Repository* repo, Product product);

// Finds and returns a pointer to a product by name and category
Product* repository_find_product(Repository* repo, const char* name, Category category);

// Searches for products containing the given string in their name
void repository_get_products_by_name(Repository* repo, const char* search_string, Product* result, size_t* result_size);

// Returns products that will expire within specified days
void repository_get_expiring_products(Repository* repo, Category category, int days, Product* result, size_t* result_size);

// Reverts the last operation performed
void repository_undo(Repository* repo);

// Reapplies the last undone operation
void repository_redo(Repository* repo); 