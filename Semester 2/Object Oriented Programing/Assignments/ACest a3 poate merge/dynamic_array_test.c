#include "dynamic_array_test.h"
#include "dynamic_array.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static time_t create_date(int year, int month, int day) {
    struct tm tm = {0};
    tm.tm_year = year - 1900;  // Years since 1900
    tm.tm_mon = month - 1;     // Months are 0-11
    tm.tm_mday = day;
    return mktime(&tm);
}

// Tests creation of dynamic arrays with various capacities
void test_dynamic_array_create() {
    printf("Testing dynamic array creation...\n");
    
    // Test with zero capacity (should use 1)
    ProductDynamicArray* arr = create_product_array(0);
    assert(arr != NULL);
    assert(arr->capacity == 1);
    assert(arr->size == 0);
    destroy_product_array(arr);
    
    // Test with valid capacity
    arr = create_product_array(10);
    assert(arr != NULL);
    assert(arr->capacity == 10);
    assert(arr->size == 0);
    destroy_product_array(arr);
    
    printf("Dynamic array creation test passed!\n");
}

// Tests adding elements and array growth
void test_dynamic_array_add() {
    printf("Testing dynamic array add...\n");
    
    ProductDynamicArray* arr = create_product_array(2);
    assert(arr != NULL);
    
    // Add products and test growth
    Product p1 = create_product("Test1", CATEGORY_DAIRY, 1.0, time(NULL));
    Product p2 = create_product("Test2", CATEGORY_DAIRY, 2.0, time(NULL));
    Product p3 = create_product("Test3", CATEGORY_DAIRY, 3.0, time(NULL));
    
    product_array_add(arr, p1);
    assert(arr->size == 1);
    assert(arr->capacity == 2);
    
    product_array_add(arr, p2);
    assert(arr->size == 2);
    assert(arr->capacity == 2);
    
    product_array_add(arr, p3);
    assert(arr->size == 3);
    assert(arr->capacity == 4);
    
    // Verify products were added correctly
    assert(strcmp(arr->items[0].name, "Test1") == 0);
    assert(strcmp(arr->items[1].name, "Test2") == 0);
    assert(strcmp(arr->items[2].name, "Test3") == 0);
    
    destroy_product_array(arr);
    printf("Dynamic array add test passed!\n");
}

// Tests removing elements from various positions
void test_dynamic_array_remove() {
    printf("Testing dynamic array remove...\n");
    
    ProductDynamicArray* arr = create_product_array(3);
    assert(arr != NULL);
    
    // Add test products
    Product p1 = create_product("Test1", CATEGORY_DAIRY, 1.0, time(NULL));
    Product p2 = create_product("Test2", CATEGORY_DAIRY, 2.0, time(NULL));
    Product p3 = create_product("Test3", CATEGORY_DAIRY, 3.0, time(NULL));
    
    product_array_add(arr, p1);
    product_array_add(arr, p2);
    product_array_add(arr, p3);
    
    // Test invalid index
    product_array_remove_at(arr, 3);
    assert(arr->size == 3);
    
    // Remove from middle
    product_array_remove_at(arr, 1);
    assert(arr->size == 2);
    assert(strcmp(arr->items[0].name, "Test1") == 0);
    assert(strcmp(arr->items[1].name, "Test3") == 0);
    
    // Remove from beginning
    product_array_remove_at(arr, 0);
    assert(arr->size == 1);
    assert(strcmp(arr->items[0].name, "Test3") == 0);
    
    // Remove last item
    product_array_remove_at(arr, 0);
    assert(arr->size == 0);
    
    destroy_product_array(arr);
    printf("Dynamic array remove test passed!\n");
}

// Tests finding elements by name and category
void test_dynamic_array_find() {
    printf("Testing dynamic array find...\n");
    
    ProductDynamicArray* arr = create_product_array(3);
    assert(arr != NULL);
    
    // Add test products
    Product p1 = create_product("Test1", CATEGORY_DAIRY, 1.0, time(NULL));
    Product p2 = create_product("Test2", CATEGORY_MEAT, 2.0, time(NULL));
    Product p3 = create_product("Test3", CATEGORY_DAIRY, 3.0, time(NULL));
    
    product_array_add(arr, p1);
    product_array_add(arr, p2);
    product_array_add(arr, p3);
    
    // Test finding existing product
    Product* found = product_array_find(arr, "Test1", CATEGORY_DAIRY);
    assert(found != NULL);
    assert(strcmp(found->name, "Test1") == 0);
    assert(found->category == CATEGORY_DAIRY);
    
    // Test finding product with wrong category
    found = product_array_find(arr, "Test1", CATEGORY_MEAT);
    assert(found == NULL);
    
    // Test finding non-existent product
    found = product_array_find(arr, "NonExistent", CATEGORY_DAIRY);
    assert(found == NULL);
    
    // Test with NULL array
    found = product_array_find(NULL, "Test1", CATEGORY_DAIRY);
    assert(found == NULL);
    
    // Test with NULL name
    found = product_array_find(arr, NULL, CATEGORY_DAIRY);
    assert(found == NULL);
    
    destroy_product_array(arr);
    printf("Dynamic array find test passed!\n");
}

// Runs all dynamic array tests
void run_all_dynamic_array_tests() {
    printf("\nRunning all dynamic array tests...\n");
    test_dynamic_array_create();
    test_dynamic_array_add();
    test_dynamic_array_remove();
    test_dynamic_array_find();
    printf("All dynamic array tests passed!\n\n");
} 