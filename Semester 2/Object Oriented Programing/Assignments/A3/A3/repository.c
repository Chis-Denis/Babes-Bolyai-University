#define _CRT_SECURE_NO_WARNINGS

#include "repository.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Repository* createRepository() {
    Repository* repo = (Repository*)malloc(sizeof(Repository));
    if (!repo) {
        printf("Memory allocation failed for Repository.\n");
        return NULL;
    }

    repo->products = createDynamicArray(10, destroyProduct);
    if (!repo->products) {
        free(repo);
        printf("Memory allocation failed for product list.\n");
        return NULL;
    }

    return repo;
}


void destroyRepository(Repository* repo) {
    destroyDynamicArray(repo->products);
    free(repo);
}

void addProduct(Repository* repo, Product* product) {
    addElementToDynamicArray(repo->products, product);
}

void removeProduct(Repository* repo, const char* name, const char* category) {
    for (int i = 0; i < getLengthOfDynamicArray(repo->products); i++) {
        Product* p = (Product*)getElementOnPosition(repo->products, i);
        if (strcmp(p->name, name) == 0 && strcmp(p->category, category) == 0) {
            deleteElementFromDynamicArray(repo->products, i);
            break;
        }
    }
}

void updateProduct(Repository* repo, const char* name, const char* category, int newQuantity, const char* newExpiration) {
    for (int i = 0; i < getLengthOfDynamicArray(repo->products); i++) {
        Product* p = (Product*)getElementOnPosition(repo->products, i);
        if (strcmp(p->name, name) == 0 && strcmp(p->category, category) == 0) {
			p->quantity = newQuantity;
			free(p->expiration_date);
			p->expiration_date = (char*)malloc(strlen(newExpiration) + 1);
			strcpy(p->expiration_date, newExpiration);
			break;
        }
    }
}

int getAllProducts(Repository* repo) {
	//no print here, print in UI
    
}

int daysBetween(const char* dateStr) {
    struct tm expDate = { 0 };
    sscanf(dateStr, "%d-%d-%d", &expDate.tm_year, &expDate.tm_mon, &expDate.tm_mday);
    expDate.tm_year -= 1900;
    expDate.tm_mon -= 1;

    time_t now = time(NULL);
    struct tm* today = localtime(&now);

    time_t expiration = mktime(&expDate);
    time_t current = mktime(today);

    return (expiration - current) / (60 * 60 * 24);
}

Product** getProductsExpiringSoon(Repository* repo, const char* category, int days, int* resultCount) {
    int count = 0;
    Product** results = malloc(sizeof(Product*) * getLengthOfDynamicArray(repo->products));

    for (int i = 0; i < getLengthOfDynamicArray(repo->products); i++) {
        Product* p = (Product*)getElementOnPosition(repo->products, i);
        if ((category == NULL || strcmp(p->category, category) == 0) && daysBetween(p->expiration_date) <= days) {
            results[count++] = p;
        }
    }
    *resultCount = count;
    return results;
}