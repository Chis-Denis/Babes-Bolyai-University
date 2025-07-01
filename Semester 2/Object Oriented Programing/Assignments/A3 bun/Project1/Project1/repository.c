#include "repository.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

Repository* createRepository()
{
    Repository* repo = (Repository*)malloc(sizeof(Repository));
    if (repo == NULL) return NULL;

    repo->dynamicArray = createDynamicArray(10, (DestroyElementFunctionPointer)destroyProduct);
    if (repo->dynamicArray == NULL) {
        free(repo);
        return NULL;
    }

    return repo;
}

DynamicArray* getDynamicArray(Repository* repository)
{
    if (repository == NULL) return NULL;
    return repository->dynamicArray;
}

int getRepositoryLength(Repository* repository)
{
    if (repository == NULL) return 0;
    return getLengthOfDynamicArray(repository->dynamicArray);
}

void destroyRepository(Repository* repository)
{
    if (repository == NULL) return;
    destroyDynamicArray(repository->dynamicArray);
    free(repository);
}

void addProductToRepository(Repository* repository, Product* product)
{
    if (repository == NULL || product == NULL) return;
    addElementToDynamicArray(repository->dynamicArray, product);
}

void deleteProductFromRepository(Repository* repository, int position)
{
    if (repository == NULL) return;
    deleteElementFromDynamicArray(repository->dynamicArray, position);
}

void updateProductFromRepository(Repository* repository, int position, Product* product)
{
    if (repository == NULL || product == NULL) return;
    updateElementOnPosition(repository->dynamicArray, position, product);
}

void sortProductBasedOnCondition(Repository* repository, OperationOfComparisonBetweenProducts operationOfComparisonBetweenProducts)
{
    for (int i = 0; i < getRepositoryLength(repository) - 1; ++i)
    {
        for (int j = i + 1; j < getRepositoryLength(repository); ++j)
        {
            Product* product1 = (Product*)getElementOnPosition(getDynamicArray(repository), i);
            Product* product2 = (Product*)getElementOnPosition(getDynamicArray(repository), j);
            if (product1 && product2 && operationOfComparisonBetweenProducts(product2, product1) > 0)
                swapElementsOnDynamicArray(getDynamicArray(repository), i, j);
        }
    }
}

Product* getProductFromRepository(Repository* repository, int position)
{
    if (repository == NULL) return NULL;
    TElem* elem = getElementOnPosition(repository->dynamicArray, position);
    return elem ? *(Product**)elem : NULL;
}

int checkIfProductIsInRepository(Repository* repository, Product* product)
{
    for (int i = 0; i < getRepositoryLength(repository); ++i)
    {
        Product* prod = getProductFromRepository(repository, i);
        if (prod && compareProducts(prod, product) == 1)
            return i;
    }
    return -1;
}

Repository* copyRepository(Repository* repository)
{
    if (repository == NULL) return NULL;

    Repository* copy = (Repository*)malloc(sizeof(Repository));
    if (copy == NULL) return NULL;

    copy->dynamicArray = copyDynamicArrayOfProducts(repository->dynamicArray);
    if (copy->dynamicArray == NULL) {
        free(copy);
        return NULL;
    }

    return copy;
}

void setRepository(Repository* destination, Repository* source)
{
    if (destination == NULL || source == NULL) return;
    destroyDynamicArray(destination->dynamicArray);
    destination->dynamicArray = copyDynamicArrayOfProducts(source->dynamicArray);
}

int isProductExpiringInXDaysRepository(Product* product, int days)
{
    if (product == NULL || days < 0) return 0;
    int daysUntilExpiration = getDaysUntilExpiration(getExpirationDateOfProduct(product));
    return daysUntilExpiration <= days && daysUntilExpiration >= 0;
}