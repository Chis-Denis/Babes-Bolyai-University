#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "services_test.h"
#include "product.h"
#include "services.h"

void testPerformOperationsOnProductService()
{
    Repository* repository = createRepository();
    UndoRedoRepository* undoRedoRepository = createUndoRedoRepository(1);
    Service* service = createService(repository, undoRedoRepository, 1);

    Product* product = createProduct("Milk", "Dairy", 10, "2025-03-30");
    assert(addProductService(service, product) == 1);

    Product* product2 = createProduct("Milk", "Dairy", 5, "2025-03-30");
    assert(addProductService(service, product2) == 0); // Quantity update

    assert(deleteProductService(service, product2) == 1);
    assert(deleteProductService(service, product2) == 0);

    destroyService(service);
}

void testAllService()
{
    testPerformOperationsOnProductService();
}
