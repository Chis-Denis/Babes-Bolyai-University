#include "product_test.h"
#include "product.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

// Tests product creation with various inputs
void test_product_create() {
    const char* name = "Test Product";
    Category category = CATEGORY_DAIRY;
    double quantity = 1.5;
    time_t expiration = time(NULL) + 86400; // tomorrow
    
    Product p = create_product(name, category, quantity, expiration);
    
    assert(strcmp(p.name, name) == 0);
    assert(p.category == category);
    assert(p.quantity == quantity);
    assert(p.expiration_date == expiration);
    
    printf("Product creation test passed!\n");
}

// Tests expiration checking functionality
void test_product_expiring_soon() {
    time_t now = time(NULL);
    Product p1 = create_product("Test1", CATEGORY_DAIRY, 1.0, now + 86400); // expires tomorrow
    Product p2 = create_product("Test2", CATEGORY_DAIRY, 1.0, now + 7 * 86400); // expires in a week
    
    assert(is_expiring_soon(&p1, 2) == 1); // should expire soon
    assert(is_expiring_soon(&p2, 2) == 0); // should not expire soon
    assert(is_expiring_soon(NULL, 2) == 0); // NULL check
    assert(is_expiring_soon(&p1, -1) == 0); // invalid days
    
    printf("Product expiration test passed!\n");
}

// Tests product equality comparison
void test_product_equality() {
    Product p1 = create_product("Test", CATEGORY_DAIRY, 1.0, time(NULL));
    Product p2 = create_product("Test", CATEGORY_DAIRY, 2.0, time(NULL)); // same name and category, different quantity
    Product p3 = create_product("Test", CATEGORY_MEAT, 1.0, time(NULL)); // different category
    Product p4 = create_product("Different", CATEGORY_DAIRY, 1.0, time(NULL)); // different name
    
    assert(products_are_equal(&p1, &p2) == 1); // should be equal (same name and category)
    assert(products_are_equal(&p1, &p3) == 0); // should be different (different category)
    assert(products_are_equal(&p1, &p4) == 0); // should be different (different name)
    assert(products_are_equal(&p1, NULL) == 0); // NULL check
    assert(products_are_equal(NULL, &p2) == 0); // NULL check
    
    printf("Product equality test passed!\n");
}

// Runs all product-related tests
void run_all_product_tests() {
    printf("Running product tests...\n");
    test_product_create();
    test_product_expiring_soon();
    test_product_equality();
    printf("All product tests passed!\n\n");
} 