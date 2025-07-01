#include "tests.h"
#include <assert.h>
#include <stdio.h>
#include "dynamic_array.h"
#include "product.h"
#include "UI.h"

void testDynamicArray() {
    DynamicArray* array = createDynamicArray(2, destroyProduct);
    assert(getLengthOfDynamicArray(array) == 0);

    Product* p1 = createProduct("Milk", "Dairy", 10, "2025-05-10");
    Product* p2 = createProduct("Cheese", "Dairy", 5, "2025-06-15");
    addElementToDynamicArray(array, p1);
    addElementToDynamicArray(array, p2);
    assert(getLengthOfDynamicArray(array) == 2);

    deleteElementFromDynamicArray(array, 0);
    assert(getLengthOfDynamicArray(array) == 1);

    destroyDynamicArray(array);
    printf("DynamicArray tests passed.\n");
}

void testProduct() {
    Product* p = createProduct("Milk", "Dairy", 10, "2025-05-10");
    assert(p != NULL);
    assert(p->quantity == 10);

    Product* p_copy = copyProduct(p);
    assert(p_copy != NULL);
    assert(p_copy->quantity == 10);

    destroyProduct(p);
    destroyProduct(p_copy);
    printf("Product tests passed.\n");
}

void testController() {
    Repository* repo = createRepository();
    Controller* controller = createController(repo);

    // Add some products
    addProductController(controller, "Milk", "Dairy", 10, "2025-05-10");
    addProductController(controller, "Yogurt", "Dairy", 5, "2025-03-20");  // Close expiration

    // Test expiration filtering
    int count;
    Product** results = getExpiringProductsController(controller, "Dairy", 30, &count);
    assert(count == 1);  // Only "Yogurt" should be included
    free(results);

    //// Test undo
    //removeProductController(controller, "Milk", "Dairy");
    //assert(getLengthOfDynamicArray(controller->repo->products) == 1);
    //undo(controller);
    //assert(getLengthOfDynamicArray(controller->repo->products) == 2);

    //// Test redo
    //redo(controller);
    //assert(getLengthOfDynamicArray(controller->repo->products) == 1);

    destroyController(controller);
    printf("Controller tests passed.\n");
}