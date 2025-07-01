#include "services.h"
#include <stdlib.h>
#include <string.h>

// Creates a new service instance with the given repository
ProductService* create_service(Repository* repo) {
    if (repo == NULL) return NULL;
    
    ProductService* service = (ProductService*)malloc(sizeof(ProductService));
    if (service == NULL) return NULL;
    
    service->repo = repo;
    return service;
}

// Frees memory used by the service (but not the repository)
void destroy_service(ProductService* service) {
    if (service != NULL) {
        free(service);
    }
}

// Initializes the repository with a set of default products
int service_init_default_products(ProductService* service) {
    if (service == NULL) return 0;
    
    time_t now = time(NULL);
    time_t tomorrow = now + 86400;
    time_t next_week = now + 7 * 86400;
    time_t next_month = now + 30 * 86400;
    
    // Initialize with default products
    int success = 1;
    success &= service_add_product(service, "Milk", CATEGORY_DAIRY, 2.0, tomorrow);
    success &= service_add_product(service, "Yogurt", CATEGORY_DAIRY, 3.0, next_week);
    success &= service_add_product(service, "Chocolate", CATEGORY_SWEETS, 1.0, next_month);
    success &= service_add_product(service, "Ice Cream", CATEGORY_SWEETS, 0.5, tomorrow);
    success &= service_add_product(service, "Chicken", CATEGORY_MEAT, 1.5, next_week);
    success &= service_add_product(service, "Beef", CATEGORY_MEAT, 2.0, tomorrow);
    success &= service_add_product(service, "Apple", CATEGORY_FRUIT, 5.0, next_week);
    success &= service_add_product(service, "Banana", CATEGORY_FRUIT, 4.0, tomorrow);
    success &= service_add_product(service, "Orange", CATEGORY_FRUIT, 3.0, next_week);
    success &= service_add_product(service, "Cheese", CATEGORY_DAIRY, 1.0, next_month);
    
    return success;
}

// Validates and adds a new product to the repository
int service_add_product(ProductService* service, const char* name, Category category, double quantity, time_t expiration_date) {
    if (service == NULL || name == NULL || quantity <= 0) return 0;
    if (category < CATEGORY_DAIRY || category > CATEGORY_FRUIT) return 0;
    if (strlen(name) == 0 || strlen(name) >= 50) return 0;
    if (expiration_date <= time(NULL)) return 0;
    
    Product product = create_product(name, category, quantity, expiration_date);
    repository_add_product(service->repo, product);
    return 1;
}

// Validates and removes a product from the repository
int service_delete_product(ProductService* service, const char* name, Category category) {
    if (service == NULL || name == NULL) return 0;
    if (category < CATEGORY_DAIRY || category > CATEGORY_FRUIT) return 0;
    
    Product* existing = repository_find_product(service->repo, name, category);
    if (existing == NULL) return 0;
    
    repository_delete_product(service->repo, name, category);
    return 1;
}

// Validates and updates an existing product's details
int service_update_product(ProductService* service, const char* name, Category category, double quantity, time_t expiration_date) {
    if (service == NULL || name == NULL || quantity <= 0) return 0;
    if (category < CATEGORY_DAIRY || category > CATEGORY_FRUIT) return 0;
    if (strlen(name) == 0 || strlen(name) >= 50) return 0;
    if (expiration_date <= time(NULL)) return 0;
    
    Product* existing = repository_find_product(service->repo, name, category);
    if (existing == NULL) return 0;
    
    Product updated = create_product(name, category, quantity, expiration_date);
    repository_update_product(service->repo, updated);
    return 1;
}

// Searches for products containing the given string in their name
void service_get_products_by_name(ProductService* service, const char* search_string, Product* result, size_t* result_size) {
    if (service == NULL || result == NULL || result_size == NULL) return;
    repository_get_products_by_name(service->repo, search_string, result, result_size);
}

// Finds products that will expire within the specified days
void service_get_expiring_products(ProductService* service, Category category, int days, Product* result, size_t* result_size) {
    if (service == NULL || result == NULL || result_size == NULL || days < 0) return;
    if (category != CATEGORY_ANY && (category < CATEGORY_DAIRY || category > CATEGORY_FRUIT)) return;
    
    repository_get_expiring_products(service->repo, category, days, result, result_size);
}

// Undoes the last operation if possible
int service_undo(ProductService* service) {
    if (service == NULL) return 0;
    repository_undo(service->repo);
    return 1;
}

// Redoes the last undone operation if possible
int service_redo(ProductService* service) {
    if (service == NULL) return 0;
    repository_redo(service->repo);
    return 1;
} 