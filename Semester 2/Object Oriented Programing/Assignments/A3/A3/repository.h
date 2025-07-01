#pragma once

#include "product.h"
#include "dynamic_array.h"

typedef struct {
    DynamicArray* products;
} Repository;

Repository* createRepository();
void destroyRepository(Repository* repo);
void addProduct(Repository* repo, Product* product);
void removeProduct(Repository* repo, const char* name, const char* category);
void updateProduct(Repository* repo, const char* name, const char* category, int newQuantity, const char* newExpiration);
int getAllProducts(Repository* repo);