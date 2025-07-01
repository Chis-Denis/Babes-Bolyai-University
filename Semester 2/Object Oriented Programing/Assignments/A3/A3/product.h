#pragma once

typedef struct {
    char* name;
    char* category;
    int quantity;
    char* expiration_date;
} Product;

Product* createProduct(const char* name, const char* category, int quantity, const char* expiration_date);
void destroyProduct(void* product);
Product* copyProduct(Product* product);