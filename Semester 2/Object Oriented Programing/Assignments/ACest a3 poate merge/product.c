#include "product.h"
#include <string.h>
#include <stdio.h>

// Creates a new product with the given properties and validates input
Product create_product(const char* name, Category category, double quantity, time_t expiration_date) {
    Product product;
    strncpy(product.name, name, sizeof(product.name) - 1);
    product.name[sizeof(product.name) - 1] = '\0';
    product.category = category;
    product.quantity = quantity;
    product.expiration_date = expiration_date;
    return product;
}

// Checks if a product will expire within the specified number of days
int is_expiring_soon(const Product* product, int days) {
    if (product == NULL || days < 0) return 0;
    
    time_t now = time(NULL);
    const int seconds_in_day = 86400; // 24 * 60 * 60
    return (product->expiration_date - now) <= (days * seconds_in_day);
}

// Compares two products for equality based on name and category
int products_are_equal(const Product* p1, const Product* p2) {
    if (p1 == NULL || p2 == NULL) return 0;
    return strcmp(p1->name, p2->name) == 0 && p1->category == p2->category;
}

// Converts a category enum to its human-readable string representation
const char* category_to_string(Category category) {
    switch (category) {
        case CATEGORY_DAIRY: return "Dairy";
        case CATEGORY_SWEETS: return "Sweets";
        case CATEGORY_MEAT: return "Meat";
        case CATEGORY_FRUIT: return "Fruit";
        default: return "Unknown";
    }
}

// Formats a product's information into a human-readable string
void product_to_string(const Product* product, char* str, size_t max_len) {
    if (product == NULL || str == NULL) return;
    
    char* time_str = ctime(&product->expiration_date);
    char date_buffer[26];
    strncpy(date_buffer, time_str, 24);
    date_buffer[24] = '\0';
    
    snprintf(str, max_len, "%-20s %-10s %-10.2f %s",
             product->name,
             category_to_string(product->category),
             product->quantity,
             date_buffer);
} 