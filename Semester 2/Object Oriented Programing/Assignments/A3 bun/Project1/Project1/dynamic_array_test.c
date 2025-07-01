#include "dynamic_array_test.h"
#include "dynamic_array.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void testDynamicArrayAll()
{
    DynamicArray* dynamicArray = createDynamicArray(1, destroyProduct);
    assert(dynamicArray);
    assert(getLengthOfDynamicArray(dynamicArray) == 0);
    assert(getCapacityOfDynamicArray(dynamicArray) == 1);

    Product* product = createProduct("Milk", "Dairy", 10, "2025-03-30");
    addElementToDynamicArray(dynamicArray, product);
    assert(getLengthOfDynamicArray(dynamicArray) == 1);
    assert(getCapacityOfDynamicArray(dynamicArray) == 1);
    assert(compareProducts((Product*)getElementOnPosition(dynamicArray, 0), product));

    Product* product1 = createProduct("Chocolate", "Sweets", 5, "2025-06-15");
    addElementToDynamicArray(dynamicArray, product1);
    assert(compareProducts((Product*)getElementOnPosition(dynamicArray, 1), product1));
    assert(getLengthOfDynamicArray(dynamicArray) == 2);
    assert(getCapacityOfDynamicArray(dynamicArray) == 2);

    Product* product2 = createProduct("Chicken", "Meat", 7, "2025-04-10");
    addElementToDynamicArray(dynamicArray, product2);

    assert(getLengthOfDynamicArray(dynamicArray) == 3);
    assert(getCapacityOfDynamicArray(dynamicArray) == 4);

    assert(!getElementOnPosition(dynamicArray, 3));
    assert(!getElementOnPosition(dynamicArray, -1));

    swapElementsOnDynamicArray(dynamicArray, 0, 2);

    assert(compareProducts((Product*)getElementOnPosition(dynamicArray, 0), product2));
    assert(compareProducts((Product*)getElementOnPosition(dynamicArray, 1), product1));
    assert(compareProducts((Product*)getElementOnPosition(dynamicArray, 2), product));

    deleteElementFromDynamicArray(dynamicArray, 2);
    assert(getLengthOfDynamicArray(dynamicArray) == 2);
    assert(getCapacityOfDynamicArray(dynamicArray) == 4);

    destroyDynamicArray(dynamicArray);
}
