#pragma once

#define CATEGORY_DAIRY "dairy"
#define CATEGORY_SWEETS "sweets"
#define CATEGORY_MEAT "meat"
#define CATEGORY_FRUIT "fruit"

typedef enum {
    INVALID_CATEGORY = -1,
    DAIRY,
    SWEETS,
    MEAT,
    FRUIT
} ProductCategory;

typedef struct
{
    char* name;
    char* category;  // dairy, sweets, meat, fruit
    int quantity;
    char* expiration_date;  // Format: YYYY-MM-DD
} Product;

typedef int (*OperationOfComparison)(Product*, char*);
typedef int (*OperationOfComparisonBetweenProducts)(Product*, Product*);

Product* createProduct(char*, char*, int, char*);
Product* copyProduct(Product*);
void destroyProduct(Product*);
char* formatProductAsString(Product*);
int compareProducts(Product*, Product*);
int productContainsString(Product*, char*);
int productDoesNotContainString(Product*, char*);
int firstProductHasNameSmallerThanSecondProduct(Product*, Product*);
int firstProductExpiresEarlierThanSecond(Product*, Product*);
int firstProductExpiresLaterThanSecond(Product*, Product*);
char* getNameOfProduct(Product*);
char* getCategoryOfProduct(Product*);
int getQuantityOfProduct(Product*);
char* getExpirationDateOfProduct(Product*);
int isValidCategory(const char* category);
int isValidDate(const char* date);
int getDaysUntilExpiration(const char* date);
