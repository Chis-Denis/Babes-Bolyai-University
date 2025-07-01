#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "product_test.h"
#include "product.h"

void testProductCreateAndDelete()
{
    Product* product = createProduct("Milk", "Dairy", 10, "2025-03-30");
    assert(product != NULL);
    assert(strcmp(getNameOfProduct(product), "Milk") == 0);
    assert(strcmp(getCategoryOfProduct(product), "Dairy") == 0);
    assert(getQuantityOfProduct(product) == 10);
    assert(strcmp(getExpirationDateOfProduct(product), "2025-03-30") == 0);
    destroyProduct(product);
}

void testProductCopy()
{
    Product* product = createProduct("Chocolate", "Sweets", 5, "2025-06-15");
    Product* copy = copyProduct(product);
    assert(copy != NULL);
    assert(strcmp(getNameOfProduct(copy), "Chocolate") == 0);
    assert(strcmp(getCategoryOfProduct(copy), "Sweets") == 0);
    assert(getQuantityOfProduct(copy) == 5);
    assert(strcmp(getExpirationDateOfProduct(copy), "2025-06-15") == 0);
    destroyProduct(product);
    destroyProduct(copy);
}

void testProductFormat()
{
    Product* product = createProduct("Chocolate", "Sweets", 5, "2025-06-15");
    char* formattedProductString = formatProductAsString(product);
    assert(formattedProductString != NULL);
    assert(strcmp(formattedProductString, "Chocolate Sweets 5 2025-06-15") == 0);
    destroyProduct(product);
    free(formattedProductString);
}

void testProductComparisons()
{
    Product* product1 = createProduct("Apple", "Fruit", 5, "2025-04-05");
    Product* product2 = createProduct("Banana", "Fruit", 6, "2025-04-06");
    Product* product3 = createProduct("Apple", "Fruit", 5, "2025-04-05");
    assert(compareProducts(product1, product2) == 0);
    assert(compareProducts(product1, product3) == 1);
    assert(firstProductHasNameSmallerThanSecondProduct(product1, product2) == 1);
    assert(firstProductExpiresEarlierThanSecond(product1, product2) == 1);
    destroyProduct(product1);
    destroyProduct(product2);
    destroyProduct(product3);
}

void testProductAll()
{
    testProductCreateAndDelete();
    testProductCopy();
    testProductFormat();
    testProductComparisons();
}
