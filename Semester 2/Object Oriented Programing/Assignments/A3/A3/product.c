#define _CRT_SECURE_NO_WARNINGS

#include "product.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Product* createProduct(const char* name, const char* category, int quantity, const char* expiration_date) {
    if (!name || !category || !expiration_date) return NULL;  // Prevent NULL access

    Product* product = (Product*)malloc(sizeof(Product));
    if (!product) {
        printf("Memory allocation failed for Product.\n");
        return NULL;
    }

    product->name = (char*)malloc(strlen(name) + 1);
    product->category = (char*)malloc(strlen(category) + 1);
    product->expiration_date = (char*)malloc(strlen(expiration_date) + 1);

    if (!product->name || !product->category || !product->expiration_date) {
        free(product->name);
        free(product->category);
        free(product->expiration_date);
        free(product);
        printf("Memory allocation failed for Product fields.\n");
        return NULL;
    }

    strcpy(product->name, name);
    strcpy(product->category, category);
    strcpy(product->expiration_date, expiration_date);
    product->quantity = quantity;

    return product;
}


void destroyProduct(void* product) {
    Product* p = (Product*)product;
    free(p->name);
    free(p->category);
    free(p->expiration_date);
    free(p);
}

Product* copyProduct(Product* product) {
    return createProduct(product->name, product->category, product->quantity, product->expiration_date);
}