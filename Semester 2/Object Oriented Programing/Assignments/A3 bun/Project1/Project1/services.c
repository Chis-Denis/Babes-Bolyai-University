#include "services.h"
#include "dynamic_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

Service* createService(Repository* repository, UndoRedoRepository* undoRedoRepository, int typeOfUndoRedo)
{
    if (repository == NULL || undoRedoRepository == NULL) return NULL;
    
    Service* service = (Service*)malloc(sizeof(Service));
    if (service == NULL) return NULL;
    
    service->repository = repository;
    service->undoRedoRepository = undoRedoRepository;
    service->typeOfUndoRedo = typeOfUndoRedo;
    return service;
}

void destroyService(Service* service)
{
    if (service == NULL) return;
    destroyRepository(service->repository);
    destroyUndoRedoRepository(service->undoRedoRepository);
    free(service);
}

int addProductService(Service* service, Product* product)
{
    if (service == NULL || product == NULL) return 0;
    
    // Check if product already exists
    for (int i = 0; i < getRepositoryLength(service->repository); i++) {
        Product* existingProduct = getProductFromRepository(service->repository, i);
        if (existingProduct == NULL) continue;
        
        if (strcmp(existingProduct->name, product->name) == 0 && 
            strcmp(existingProduct->category, product->category) == 0) {
            // Update quantity
            Product* updatedProduct = createProduct(
                existingProduct->name,
                existingProduct->category,
                existingProduct->quantity + product->quantity,
                existingProduct->expiration_date
            );
            if (updatedProduct == NULL) return 0;
            
            // Save state for undo/redo
            Repository* oldState = copyRepository(service->repository);
            if (oldState == NULL) {
                destroyProduct(updatedProduct);
                return 0;
            }
            
            updateProductFromRepository(service->repository, i, updatedProduct);
            addUndoRedoOperation(service->undoRedoRepository, oldState, NULL);
            destroyProduct(updatedProduct);
            return 1;
        }
    }
    
    // Add new product
    Repository* oldState = copyRepository(service->repository);
    if (oldState == NULL) return 0;
    
    addProductToRepository(service->repository, product);
    addUndoRedoOperation(service->undoRedoRepository, oldState, NULL);
    return 1;
}

int deleteProductService(Service* service, Product* product)
{
    if (service == NULL || product == NULL) return 0;
    
    for (int i = 0; i < getRepositoryLength(service->repository); i++) {
        Product* existingProduct = getProductFromRepository(service->repository, i);
        if (existingProduct == NULL) continue;
        
        if (strcmp(existingProduct->name, product->name) == 0 && 
            strcmp(existingProduct->category, product->category) == 0) {
            Repository* oldState = copyRepository(service->repository);
            if (oldState == NULL) return 0;
            
            deleteProductFromRepository(service->repository, i);
            addUndoRedoOperation(service->undoRedoRepository, oldState, NULL);
            return 1;
        }
    }
    return 0;
}

int updateProductService(Service* service, Product* oldProduct, Product* newProduct)
{
    if (service == NULL || oldProduct == NULL || newProduct == NULL) return 0;
    
    for (int i = 0; i < getRepositoryLength(service->repository); i++) {
        Product* existingProduct = getProductFromRepository(service->repository, i);
        if (existingProduct == NULL) continue;
        
        if (strcmp(existingProduct->name, oldProduct->name) == 0 && 
            strcmp(existingProduct->category, oldProduct->category) == 0) {
            Repository* oldState = copyRepository(service->repository);
            if (oldState == NULL) return 0;
            
            updateProductFromRepository(service->repository, i, newProduct);
            addUndoRedoOperation(service->undoRedoRepository, oldState, NULL);
            return 1;
        }
    }
    return 0;
}

int getProductsBasedOnFilteringService(Service* service, char* searchString, OperationOfComparison filterFunction)
{
    if (service == NULL) return 0;
    
    // First, filter products based on search string
    DynamicArray* filteredArray = createDynamicArray(10, (DestroyElementFunctionPointer)destroyProduct);
    for (int i = 0; i < getRepositoryLength(service->repository); i++) {
        Product* product = getProductFromRepository(service->repository, i);
        if (strlen(searchString) == 0 || filterFunction(product, searchString)) {
            addElementToDynamicArray(filteredArray, copyProduct(product));
        }
    }
    
    // Sort by quantity
    for (int i = 0; i < getLengthOfDynamicArray(filteredArray) - 1; i++) {
        for (int j = i + 1; j < getLengthOfDynamicArray(filteredArray); j++) {
            Product* p1 = (Product*)getElementOnPosition(filteredArray, i);
            Product* p2 = (Product*)getElementOnPosition(filteredArray, j);
            if (p1->quantity > p2->quantity) {
                swapElementsOnDynamicArray(filteredArray, i, j);
            }
        }
    }
    
    // Print results
    printf("\nFiltered and sorted products:\n");
    for (int i = 0; i < getLengthOfDynamicArray(filteredArray); i++) {
        Product* product = (Product*)getElementOnPosition(filteredArray, i);
        char* str = formatProductAsString(product);
        printf("%s\n", str);
        free(str);
    }
    
    destroyDynamicArray(filteredArray);
    return 1;
}

int getProductsExpiringInXDaysService(Service* service, int days)
{
    if (service == NULL || days < 0) return 0;
    
    printf("\nProducts expiring in %d days:\n", days);
    int found = 0;
    
    for (int i = 0; i < getRepositoryLength(service->repository); i++) {
        Product* product = getProductFromRepository(service->repository, i);
        int daysUntilExpiration = getDaysUntilExpiration(getExpirationDateOfProduct(product));
        
        if (daysUntilExpiration <= days && daysUntilExpiration >= 0) {
            char* str = formatProductAsString(product);
            printf("%s\n", str);
            free(str);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No products found expiring in the next %d days.\n", days);
    }
    
    return 1;
}

DynamicArray* filterProductsByCategory(Service* service, const char* category)
{
    if (service == NULL || category == NULL) return NULL;
    
    DynamicArray* result = createDynamicArray();
    if (result == NULL) return NULL;
    
    for (int i = 0; i < getRepositoryLength(service->repository); i++) {
        Product* product = getProductFromRepository(service->repository, i);
        if (product == NULL) continue;
        
        if (strcmp(product->category, category) == 0) {
            Product* copy = createProduct(
                product->name,
                product->category,
                product->quantity,
                product->expiration_date
            );
            if (copy == NULL) {
                destroyDynamicArray(result);
                return NULL;
            }
            addElementToDynamicArray(result, copy);
        }
    }
    return result;
}

DynamicArray* filterProductsByQuantity(Service* service, int quantity)
{
    if (service == NULL) return NULL;
    
    DynamicArray* result = createDynamicArray();
    if (result == NULL) return NULL;
    
    for (int i = 0; i < getRepositoryLength(service->repository); i++) {
        Product* product = getProductFromRepository(service->repository, i);
        if (product == NULL) continue;
        
        if (product->quantity < quantity) {
            Product* copy = createProduct(
                product->name,
                product->category,
                product->quantity,
                product->expiration_date
            );
            if (copy == NULL) {
                destroyDynamicArray(result);
                return NULL;
            }
            addElementToDynamicArray(result, copy);
        }
    }
    return result;
}

DynamicArray* filterExpiredProducts(Service* service, const char* currentDate)
{
    if (service == NULL || currentDate == NULL) return NULL;
    
    DynamicArray* result = createDynamicArray();
    if (result == NULL) return NULL;
    
    for (int i = 0; i < getRepositoryLength(service->repository); i++) {
        Product* product = getProductFromRepository(service->repository, i);
        if (product == NULL) continue;
        
        if (strcmp(product->expiration_date, currentDate) < 0) {
            Product* copy = createProduct(
                product->name,
                product->category,
                product->quantity,
                product->expiration_date
            );
            if (copy == NULL) {
                destroyDynamicArray(result);
                return NULL;
            }
            addElementToDynamicArray(result, copy);
        }
    }
    return result;
}

int undoService(Service* service)
{
    if (service == NULL) return 0;
    return undoOperation(service->undoRedoRepository);
}

int redoService(Service* service)
{
    if (service == NULL) return 0;
    return redoOperation(service->undoRedoRepository);
}

int getTypeOfUndoRedo(Service* service)
{
    return service->typeOfUndoRedo;
}

void generateRandomEntries(Service* service, int count)
{
    if (service == NULL || count <= 0) return;
    
    const char* categories[] = {"Dairy", "Meat", "Vegetables", "Fruits", "Beverages"};
    const char* names[] = {"Milk", "Cheese", "Chicken", "Beef", "Carrot", "Tomato", "Apple", "Orange", "Water", "Juice"};
    const int numCategories = sizeof(categories) / sizeof(categories[0]);
    const int numNames = sizeof(names) / sizeof(names[0]);
    
    for (int i = 0; i < count; i++) {
        char expirationDate[11];
        sprintf(expirationDate, "2024-%02d-%02d", (rand() % 12) + 1, (rand() % 28) + 1);
        
        Product* product = createProduct(
            names[rand() % numNames],
            categories[rand() % numCategories],
            (rand() % 10) + 1,
            expirationDate
        );
        
        if (product != NULL) {
            addProductService(service, product);
            destroyProduct(product);
        }
    }
}
