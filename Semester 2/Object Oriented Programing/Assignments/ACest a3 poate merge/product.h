#pragma once
#include <time.h>

// Available product categories in the refrigerator
typedef enum {
    CATEGORY_DAIRY,
    CATEGORY_SWEETS,
    CATEGORY_MEAT,
    CATEGORY_FRUIT
} Category;

// Basic product information structure
typedef struct {
    char name[50];
    Category category;
    double quantity;
    time_t expiration_date;
} Product;

// Creates a new product with the specified details
Product create_product(const char* name, Category category, double quantity, time_t expiration_date);

// Checks if a product will expire within the given number of days
int is_expiring_soon(const Product* product, int days);

// Compares two products for equality based on name and category
int products_are_equal(const Product* p1, const Product* p2);

// Converts a category enum to its string representation
const char* category_to_string(Category category);

// Formats a product's information into a string
void product_to_string(const Product* product, char* str, size_t max_len); 