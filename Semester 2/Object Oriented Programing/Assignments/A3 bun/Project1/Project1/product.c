#include "product.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int isValidCategory(const char* category) {
    return (strcmp(category, CATEGORY_DAIRY) == 0 ||
            strcmp(category, CATEGORY_SWEETS) == 0 ||
            strcmp(category, CATEGORY_MEAT) == 0 ||
            strcmp(category, CATEGORY_FRUIT) == 0);
}

int isValidDate(const char* date) {
    if (strlen(date) != 10) return 0;
    if (date[4] != '-' || date[7] != '-') return 0;
    
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) return 0;
    
    if (year < 2024 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    // Simple month length validation
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return 0;
    if (month == 2) {
        int isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) return 0;
    }
    
    return 1;
}

int getDaysUntilExpiration(const char* date) {
    time_t now;
    struct tm tm_exp = {0};
    time(&now);
    
    int year, month, day;
    sscanf(date, "%d-%d-%d", &year, &month, &day);
    
    tm_exp.tm_year = year - 1900;
    tm_exp.tm_mon = month - 1;
    tm_exp.tm_mday = day;
    
    time_t exp_time = mktime(&tm_exp);
    double diff = difftime(exp_time, now);
    return (int)(diff / (60 * 60 * 24));
}

Product* createProduct(char* name, char* category, int quantity, char* expiration_date)
{
    if (!isValidCategory(category)) {
        return NULL;
    }
    if (!isValidDate(expiration_date)) {
        return NULL;
    }
    if (quantity < 0) {
        return NULL;
    }
    
    Product* product = (Product*)malloc(sizeof(Product));
    if (product == NULL) {
        return NULL;
    }
    
    product->name = (char*)malloc(strlen(name) + 1);
    product->category = (char*)malloc(strlen(category) + 1);
    product->expiration_date = (char*)malloc(strlen(expiration_date) + 1);
    
    if (product->name == NULL || product->category == NULL || product->expiration_date == NULL) {
        free(product->name);
        free(product->category);
        free(product->expiration_date);
        free(product);
        return NULL;
    }
    
    strcpy(product->name, name);
    strcpy(product->category, category);
    strcpy(product->expiration_date, expiration_date);
    product->quantity = quantity;
    
    return product;
}

Product* copyProduct(Product* product)
{
    return createProduct(getNameOfProduct(product), getCategoryOfProduct(product), getQuantityOfProduct(product), getExpirationDateOfProduct(product));
}

char* getNameOfProduct(Product* product)
{
    return product->name;
}

char* getCategoryOfProduct(Product* product)
{
    return product->category;
}

int getQuantityOfProduct(Product* product)
{
    return product->quantity;
}

char* getExpirationDateOfProduct(Product* product)
{
    return product->expiration_date;
}

int productContainsString(Product* product, char* string)
{
    return (strstr(getNameOfProduct(product), string) != NULL);
}

int productDoesNotContainString(Product* product, char* string)
{
    return !(productContainsString(product, string));
}

void destroyProduct(Product* product)
{
    if (product)
    {
        free(product->name);
        free(product->category);
        free(product->expiration_date);
        free(product);
    }
}

char* formatProductAsString(Product* product)
{
    char* str = (char*)malloc(200);
    if (str == NULL) return NULL;
    
    int days = getDaysUntilExpiration(product->expiration_date);
    sprintf(str, "Name: %s | Category: %s | Quantity: %d | Expires: %s (%d days)", 
            product->name, product->category, product->quantity, 
            product->expiration_date, days);
    return str;
}

int compareProducts(Product* product1, Product* product2)
{
    return strcmp(getNameOfProduct(product1), getNameOfProduct(product2)) == 0 && strcmp(getCategoryOfProduct(product1), getCategoryOfProduct(product2)) == 0;
}

int firstProductHasNameSmallerThanSecondProduct(Product* product1, Product* product2)
{
    return strcmp(getNameOfProduct(product1), getNameOfProduct(product2)) < 0;
}

int firstProductExpiresEarlierThanSecond(Product* product1, Product* product2)
{
    return strcmp(getExpirationDateOfProduct(product1), getExpirationDateOfProduct(product2)) < 0;
}

int firstProductExpiresLaterThanSecond(Product* product1, Product* product2)
{
    return strcmp(getExpirationDateOfProduct(product1), getExpirationDateOfProduct(product2)) > 0;
}
