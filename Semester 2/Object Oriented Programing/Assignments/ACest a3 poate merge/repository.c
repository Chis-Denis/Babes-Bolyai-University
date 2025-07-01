#include "repository.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define CATEGORY_ANY ((Category)-1)

// Creates a new repository with empty product list and operation stacks
Repository* create_repository() {
    Repository* repo = (Repository*)malloc(sizeof(Repository));
    if (repo == NULL) return NULL;
    
    repo->products = create_product_array(INITIAL_CAPACITY);
    repo->undo_stack = create_operation_array(INITIAL_CAPACITY);
    repo->redo_stack = create_operation_array(INITIAL_CAPACITY);
    
    if (repo->products == NULL || repo->undo_stack == NULL || repo->redo_stack == NULL) {
        destroy_repository(repo);
        return NULL;
    }
    return repo;
}

// Frees all memory associated with the repository and its components
void destroy_repository(Repository* repo) {
    if (repo != NULL) {
        if (repo->products != NULL) destroy_product_array(repo->products);
        if (repo->undo_stack != NULL) destroy_operation_array(repo->undo_stack);
        if (repo->redo_stack != NULL) destroy_operation_array(repo->redo_stack);
        free(repo);
    }
}

// Clears the redo stack when a new operation is performed
static void clear_redo_stack(Repository* repo) {
    if (repo != NULL && repo->redo_stack != NULL) {
        repo->redo_stack->size = 0;
    }
}

// Adds a new product or updates quantity if it exists
void repository_add_product(Repository* repo, Product product) {
    if (repo == NULL) return;
    
    Product* existing = repository_find_product(repo, product.name, product.category);
    Operation op;
    
    if (existing) {
        Product before = *existing;
        existing->quantity += product.quantity;
        op = create_operation(OPERATION_ADD, before, *existing, 1);
    } else {
        product_array_add(repo->products, product);
        op = create_operation(OPERATION_ADD, product, product, 0);
    }
    
    operation_array_add(repo->undo_stack, op);
    clear_redo_stack(repo);
}

// Removes a product from the repository by name and category
void repository_delete_product(Repository* repo, const char* name, Category category) {
    if (repo == NULL || name == NULL) return;
    
    for (size_t i = 0; i < repo->products->size; i++) {
        Product* current = &repo->products->items[i];
        if (strcmp(current->name, name) == 0 && current->category == category) {
            Operation op = create_operation(OPERATION_DELETE, *current, *current, 0);
            operation_array_add(repo->undo_stack, op);
            product_array_remove_at(repo->products, i);
            clear_redo_stack(repo);
            break;
        }
    }
}

// Updates all fields of an existing product
void repository_update_product(Repository* repo, Product product) {
    if (repo == NULL) return;
    
    Product* existing = repository_find_product(repo, product.name, product.category);
    if (existing) {
        Operation op = create_operation(OPERATION_UPDATE, *existing, product, 0);
        *existing = product;
        operation_array_add(repo->undo_stack, op);
        clear_redo_stack(repo);
    }
}

// Finds and returns a pointer to a product by name and category
Product* repository_find_product(Repository* repo, const char* name, Category category) {
    if (repo == NULL || name == NULL) return NULL;
    return product_array_find(repo->products, name, category);
}

// Searches for products containing the given string in their name
void repository_get_products_by_name(Repository* repo, const char* search_string, Product* result, size_t* result_size) {
    if (repo == NULL || result == NULL || result_size == NULL) return;
    
    *result_size = 0;
    if (search_string == NULL) search_string = "";
    
    for (size_t i = 0; i < repo->products->size; i++) {
        if (!search_string[0] || strstr(repo->products->items[i].name, search_string)) {
            result[(*result_size)++] = repo->products->items[i];
        }
    }
    
    // Sort by quantity
    for (size_t i = 0; i < *result_size - 1; i++) {
        for (size_t j = 0; j < *result_size - i - 1; j++) {
            if (result[j].quantity > result[j + 1].quantity) {
                Product temp = result[j];
                result[j] = result[j + 1];
                result[j + 1] = temp;
            }
        }
    }
}

// Returns products that will expire within specified days
void repository_get_expiring_products(Repository* repo, Category category, int days, Product* result, size_t* result_size) {
    if (repo == NULL || result == NULL || result_size == NULL || days < 0) return;
    
    *result_size = 0;
    
    for (size_t i = 0; i < repo->products->size; i++) {
        Product* current = &repo->products->items[i];
        if ((category == CATEGORY_ANY || current->category == category) && 
            is_expiring_soon(current, days)) {
            result[(*result_size)++] = *current;
        }
    }
}

// Reverts the last operation performed
void repository_undo(Repository* repo) {
    if (repo == NULL || repo->undo_stack->size == 0) return;
    
    Operation last_op = repo->undo_stack->items[repo->undo_stack->size - 1];
    repo->undo_stack->size--;
    
    switch (last_op.type) {
        case OPERATION_ADD:
            if (last_op.is_update) {
                Product* product = repository_find_product(repo, last_op.before.name, last_op.before.category);
                if (product) product->quantity = last_op.before.quantity;
            } else {
                repository_delete_product(repo, last_op.after.name, last_op.after.category);
            }
            break;
            
        case OPERATION_DELETE:
            product_array_add(repo->products, last_op.before);
            break;
            
        case OPERATION_UPDATE:
            {
                Product* product = repository_find_product(repo, last_op.before.name, last_op.before.category);
                if (product) *product = last_op.before;
            }
            break;
    }
    
    operation_array_add(repo->redo_stack, last_op);
}

// Reapplies the last undone operation
void repository_redo(Repository* repo) {
    if (repo == NULL || repo->redo_stack->size == 0) return;
    
    Operation last_op = repo->redo_stack->items[repo->redo_stack->size - 1];
    repo->redo_stack->size--;
    
    switch (last_op.type) {
        case OPERATION_ADD:
            if (last_op.is_update) {
                Product* product = repository_find_product(repo, last_op.after.name, last_op.after.category);
                if (product) product->quantity = last_op.after.quantity;
            } else {
                product_array_add(repo->products, last_op.after);
            }
            break;
            
        case OPERATION_DELETE:
            {
                // Directly remove the product without creating a new operation
                for (size_t i = 0; i < repo->products->size; i++) {
                    if (strcmp(repo->products->items[i].name, last_op.before.name) == 0 &&
                        repo->products->items[i].category == last_op.before.category) {
                        product_array_remove_at(repo->products, i);
                        break;
                    }
                }
            }
            break;
            
        case OPERATION_UPDATE:
            {
                Product* product = repository_find_product(repo, last_op.after.name, last_op.after.category);
                if (product) *product = last_op.after;
            }
            break;
    }
    
    operation_array_add(repo->undo_stack, last_op);
} 