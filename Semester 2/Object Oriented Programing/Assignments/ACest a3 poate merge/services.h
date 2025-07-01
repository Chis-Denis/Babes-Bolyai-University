#pragma once
#include "repository.h"

// Service layer that handles business logic and validation
typedef struct {
    Repository* repo;    // Repository for data storage
} ProductService;

/**
 * Creates a new product service.
 * @param repo The repository to use for storage (must not be NULL)
 * @return Pointer to the new service, or NULL if allocation failed
 */
ProductService* create_service(Repository* repo);

/**
 * Frees all resources used by the service.
 * @param service The service to destroy
 */
void destroy_service(ProductService* service);

/**
 * Initializes the service with predefined products.
 * @param service The service to initialize
 * @return 1 if successful, 0 if failed
 */
int service_init_default_products(ProductService* service);

/**
 * Adds a new product to the service.
 * @param service The service to add to
 * @param name Product name (must not be NULL)
 * @param category Product category
 * @param quantity Product quantity (must be positive)
 * @param expiration_date Product expiration date
 * @return 1 if successful, 0 if failed
 */
int service_add_product(ProductService* service, const char* name, Category category, double quantity, time_t expiration_date);

/**
 * Deletes a product from the service.
 * @param service The service to delete from
 * @param name Product name to delete (must not be NULL)
 * @param category Product category to delete
 * @return 1 if successful, 0 if product not found
 */
int service_delete_product(ProductService* service, const char* name, Category category);

/**
 * Updates a product in the service.
 * @param service The service to update in
 * @param name Product name to update (must not be NULL)
 * @param category Product category to update
 * @param quantity New quantity (must be positive)
 * @param expiration_date New expiration date
 * @return 1 if successful, 0 if product not found
 */
int service_update_product(ProductService* service, const char* name, Category category, double quantity, time_t expiration_date);

/**
 * Searches for products by name.
 * @param service The service to search in
 * @param search_string String to search for (empty for all products)
 * @param result Array to store found products
 * @param result_size Number of products found
 */
void service_get_products_by_name(ProductService* service, const char* search_string, Product* result, size_t* result_size);

/**
 * Gets products expiring within specified days.
 * @param service The service to search in
 * @param category Category to filter by (CATEGORY_ANY for all)
 * @param days Number of days to check
 * @param result Array to store found products
 * @param result_size Number of products found
 */
void service_get_expiring_products(ProductService* service, Category category, int days, Product* result, size_t* result_size);

/**
 * Undoes the last operation.
 * @param service The service to perform undo on
 * @return 1 if successful, 0 if nothing to undo
 */
int service_undo(ProductService* service);

/**
 * Redoes the last undone operation.
 * @param service The service to perform redo on
 * @return 1 if successful, 0 if nothing to redo
 */
int service_redo(ProductService* service); 