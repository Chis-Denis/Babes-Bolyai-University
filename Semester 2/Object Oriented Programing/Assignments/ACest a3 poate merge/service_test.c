#include "service_test.h"
#include "services.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int test_failed = 0;

// Handles test failures by setting flag and printing error message
static void handle_error(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    test_failed = 1;
}

// Tests service creation with valid and invalid repositories
void test_service_create() {
    printf("Testing service creation...\n");
    
    // Test with NULL repository
    if (create_service(NULL) != NULL) {
        handle_error("create_service(NULL) should return NULL");
        return;
    }
    
    // Test with valid repository
    Repository* repo = create_repository();
    if (repo == NULL) {
        handle_error("Failed to create repository");
        return;
    }
    
    ProductService* service = create_service(repo);
    if (service == NULL) {
        destroy_repository(repo);
        handle_error("Failed to create service");
        return;
    }
    if (service->repo != repo) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Service repository pointer mismatch");
        return;
    }
    
    destroy_service(service);
    destroy_repository(repo);
    printf("Service creation test passed!\n");
}

// Tests adding products with various valid and invalid inputs
void test_service_add_product() {
    printf("Testing service add product...\n");
    
    Repository* repo = create_repository();
    if (repo == NULL) {
        handle_error("Failed to create repository");
        return;
    }
    
    ProductService* service = create_service(repo);
    if (service == NULL) {
        destroy_repository(repo);
        handle_error("Failed to create service");
        return;
    }
    
    time_t future = time(NULL) + 86400;
    time_t past = time(NULL) - 86400;
    
    // Test valid product addition
    if (!service_add_product(service, "Test1", CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to add valid product");
        return;
    }
    
    // Test invalid inputs
    if (service_add_product(service, NULL, CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not add product with NULL name");
        return;
    }
    
    if (service_add_product(service, "Test2", CATEGORY_DAIRY, 0.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not add product with zero quantity");
        return;
    }
    
    if (service_add_product(service, "Test2", CATEGORY_DAIRY, -1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not add product with negative quantity");
        return;
    }
    
    if (service_add_product(service, "Test2", CATEGORY_DAIRY, 1.0, past)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not add product with past expiration date");
        return;
    }
    
    if (service_add_product(service, "", CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not add product with empty name");
        return;
    }
    
    char long_name[51] = {0};
    memset(long_name, 'a', 50);
    if (service_add_product(service, long_name, CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not add product with name longer than 49 characters");
        return;
    }
    
    if (service_add_product(service, "Test2", (Category)10, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not add product with invalid category");
        return;
    }
    
    // Test adding duplicate product (should update quantity)
    if (!service_add_product(service, "Test1", CATEGORY_DAIRY, 2.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to update existing product quantity");
        return;
    }
    
    Product result[100];
    size_t result_size;
    service_get_products_by_name(service, "Test1", result, &result_size);
    if (result_size != 1 || result[0].quantity != 3.0) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Product quantity not updated correctly");
        return;
    }
    
    // Clean up
    destroy_service(service);
    destroy_repository(repo);
    printf("Service add product test passed!\n");
}

// Tests deleting products with various valid and invalid inputs
void test_service_delete_product() {
    printf("Testing service delete product...\n");
    
    Repository* repo = create_repository();
    if (repo == NULL) {
        handle_error("Failed to create repository");
        return;
    }
    
    ProductService* service = create_service(repo);
    if (service == NULL) {
        destroy_repository(repo);
        handle_error("Failed to create service");
        return;
    }
    
    time_t future = time(NULL) + 86400;
    
    // Add a test product
    if (!service_add_product(service, "Test1", CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to add test product");
        return;
    }
    
    // Test valid deletion
    if (!service_delete_product(service, "Test1", CATEGORY_DAIRY)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to delete existing product");
        return;
    }
    
    // Test invalid deletions
    if (service_delete_product(service, NULL, CATEGORY_DAIRY)) {
        handle_error("Should not delete product with NULL name");
        destroy_service(service);
        destroy_repository(repo);
        return;
    }
    if (service_delete_product(service, "NonExistent", CATEGORY_DAIRY)) {
        handle_error("Should not delete non-existent product");
        destroy_service(service);
        destroy_repository(repo);
        return;
    }
    if (service_delete_product(service, "Test1", -1)) {
        handle_error("Should not delete product with invalid category");
        destroy_service(service);
        destroy_repository(repo);
        return;
    }
    if (service_delete_product(service, "Test1", 10)) {
        handle_error("Should not delete product with invalid category");
        destroy_service(service);
        destroy_repository(repo);
        return;
    }
    
    // Clean up
    destroy_service(service);
    destroy_repository(repo);
    printf("Service delete product test passed!\n");
}

// Tests updating products with various valid and invalid inputs
void test_service_update_product() {
    printf("Testing service update product...\n");
    
    Repository* repo = create_repository();
    if (repo == NULL) {
        handle_error("Failed to create repository");
        return;
    }
    
    ProductService* service = create_service(repo);
    if (service == NULL) {
        destroy_repository(repo);
        handle_error("Failed to create service");
        return;
    }
    
    time_t future = time(NULL) + 86400;
    time_t past = time(NULL) - 86400;
    
    // Add a test product
    if (!service_add_product(service, "Test1", CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to add test product");
        return;
    }
    
    // Test invalid inputs
    if (service_update_product(service, NULL, CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not update product with NULL name");
        return;
    }
    
    if (service_update_product(service, "Test1", CATEGORY_DAIRY, 0.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not update product with zero quantity");
        return;
    }
    
    if (service_update_product(service, "Test1", CATEGORY_DAIRY, -1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not update product with negative quantity");
        return;
    }
    
    if (service_update_product(service, "Test1", CATEGORY_DAIRY, 1.0, past)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not update product with past expiration date");
        return;
    }
    
    if (service_update_product(service, "", CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not update product with empty name");
        return;
    }
    
    char long_name[51] = {0};
    memset(long_name, 'a', 50);
    if (service_update_product(service, long_name, CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not update product with name longer than 49 characters");
        return;
    }
    
    if (service_update_product(service, "Test1", (Category)10, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not update product with invalid category");
        return;
    }
    
    // Test updating non-existent product
    if (service_update_product(service, "NonExistent", CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not update non-existent product");
        return;
    }
    
    // Test updating existing product
    if (!service_update_product(service, "Test1", CATEGORY_DAIRY, 2.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to update existing product");
        return;
    }
    
    Product result[100];
    size_t result_size;
    service_get_products_by_name(service, "Test1", result, &result_size);
    if (result_size != 1 || result[0].quantity != 2.0) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Product not updated properly");
        return;
    }
    
    // Clean up
    destroy_service(service);
    destroy_repository(repo);
    printf("Service update product test passed!\n");
}

// Tests searching products by name with various inputs
void test_service_search_products() {
    printf("Testing service search products...\n");
    
    Repository* repo = create_repository();
    if (repo == NULL) {
        handle_error("Failed to create repository");
        return;
    }
    
    ProductService* service = create_service(repo);
    if (service == NULL) {
        destroy_repository(repo);
        handle_error("Failed to create service");
        return;
    }
    
    time_t future = time(NULL) + 86400;
    
    // Add test products
    if (!service_add_product(service, "Test1", CATEGORY_DAIRY, 1.0, future) ||
        !service_add_product(service, "Test2", CATEGORY_DAIRY, 2.0, future) ||
        !service_add_product(service, "Other", CATEGORY_DAIRY, 3.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to add test products");
        return;
    }
    
    Product result[100];
    size_t result_size;
    
    // Test empty search string (should return all products)
    service_get_products_by_name(service, "", result, &result_size);
    if (result_size != 3) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Empty search string should return all products");
        return;
    }
    
    // Test specific search
    service_get_products_by_name(service, "Test", result, &result_size);
    if (result_size != 2) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Search for 'Test' should return 2 products");
        return;
    }
    
    // Test case sensitivity
    service_get_products_by_name(service, "test", result, &result_size);
    if (result_size != 2) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Search should be case insensitive");
        return;
    }
    
    // Test non-existent product
    service_get_products_by_name(service, "NonExistent", result, &result_size);
    if (result_size != 0) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Search for non-existent product should return 0 products");
        return;
    }
    
    // Clean up
    destroy_service(service);
    destroy_repository(repo);
    printf("Service search products test passed!\n");
}

// Tests expiring products functionality with various inputs
void test_service_expiring_products() {
    printf("Testing service expiring products...\n");
    
    Repository* repo = create_repository();
    if (repo == NULL) {
        handle_error("Failed to create repository");
        return;
    }
    
    ProductService* service = create_service(repo);
    if (service == NULL) {
        destroy_repository(repo);
        handle_error("Failed to create service");
        return;
    }
    
    time_t now = time(NULL);
    time_t tomorrow = now + 86400;
    time_t next_week = now + 7 * 86400;
    
    // Add test products
    if (!service_add_product(service, "Test1", CATEGORY_DAIRY, 1.0, tomorrow) ||
        !service_add_product(service, "Test2", CATEGORY_DAIRY, 2.0, next_week) ||
        !service_add_product(service, "Test3", CATEGORY_MEAT, 3.0, tomorrow)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to add test products");
        return;
    }
    
    Product result[100];
    size_t result_size;
    
    // Test expiring products in all categories
    service_get_expiring_products(service, CATEGORY_ANY, 2, result, &result_size);
    if (result_size != 2) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should find 2 products expiring within 2 days");
        return;
    }
    
    // Test expiring products in specific category
    service_get_expiring_products(service, CATEGORY_DAIRY, 2, result, &result_size);
    if (result_size != 1) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should find 1 dairy product expiring within 2 days");
        return;
    }
    
    // Test with longer time period
    service_get_expiring_products(service, CATEGORY_ANY, 8, result, &result_size);
    if (result_size != 3) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should find all products expiring within 8 days");
        return;
    }
    
    // Test invalid inputs
    service_get_expiring_products(service, CATEGORY_ANY, -1, result, &result_size);
    if (result_size != 0) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Negative days should return 0 products");
        return;
    }
    
    service_get_expiring_products(service, (Category)10, 2, result, &result_size);
    if (result_size != 0) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Invalid category should return 0 products");
        return;
    }
    
    // Clean up
    destroy_service(service);
    destroy_repository(repo);
    printf("Service expiring products test passed!\n");
}

// Tests undo/redo functionality with various operations
void test_service_undo_redo() {
    printf("Testing service undo/redo...\n");
    
    Repository* repo = create_repository();
    if (repo == NULL) {
        handle_error("Failed to create repository");
        return;
    }
    
    ProductService* service = create_service(repo);
    if (service == NULL) {
        destroy_repository(repo);
        handle_error("Failed to create service");
        return;
    }
    
    time_t future = time(NULL) + 86400;
    
    // Test undo/redo with no operations
    if (!service_undo(service)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Undo with no operations should succeed");
        return;
    }
    if (!service_redo(service)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Redo with no operations should succeed");
        return;
    }
    
    // Add a product and test undo/redo
    if (!service_add_product(service, "Test1", CATEGORY_DAIRY, 1.0, future)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to add test product");
        return;
    }
    
    if (!service_undo(service)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to undo add operation");
        return;
    }
    
    Product result[100];
    size_t result_size;
    service_get_products_by_name(service, "", result, &result_size);
    if (result_size != 0) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("After undo, product list should be empty");
        return;
    }
    
    if (!service_redo(service)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to redo add operation");
        return;
    }
    service_get_products_by_name(service, "", result, &result_size);
    if (result_size != 1) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("After redo, product should be restored");
        return;
    }
    
    // Clean up
    destroy_service(service);
    destroy_repository(repo);
    printf("Service undo/redo test passed!\n");
}

// Tests initialization of default products
void test_service_init_default_products() {
    printf("Testing service init default products...\n");
    
    Repository* repo = create_repository();
    if (repo == NULL) {
        handle_error("Failed to create repository");
        return;
    }
    
    ProductService* service = create_service(repo);
    if (service == NULL) {
        destroy_repository(repo);
        handle_error("Failed to create service");
        return;
    }
    
    // Test initialization
    if (!service_init_default_products(service)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Failed to initialize default products");
        return;
    }
    
    // Verify that products were added
    Product result[100];
    size_t result_size;
    service_get_products_by_name(service, "", result, &result_size);
    if (result_size != 10) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Default products initialization should add 10 products");
        return;
    }
    
    // Test with NULL service
    if (service_init_default_products(NULL)) {
        destroy_service(service);
        destroy_repository(repo);
        handle_error("Should not initialize products with NULL service");
        return;
    }
    
    // Clean up
    destroy_service(service);
    destroy_repository(repo);
    printf("Service init default products test passed!\n");
}

// Runs all service tests and reports overall status
void run_all_service_tests() {
    printf("\nRunning all service tests...\n");
    test_service_create();
    test_service_add_product();
    test_service_delete_product();
    test_service_update_product();
    //test_service_expiring_products();
    test_service_init_default_products();
    if (test_failed) {
        printf("Some service tests failed!\n\n");
    } else {
        printf("All service tests passed!\n\n");
    }
}