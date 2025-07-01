#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[100];        // Name of the product
    char category[20];     // Category (dairy, sweets, meat, fruit)
    int quantity;          // Quantity in stock
    char expiration_date[20]; // Expiration date
} Product;

#define MAX_PRODUCTS 1000
Product products[MAX_PRODUCTS];
int product_count = 0;

// Finds a product by name and category, returns its index or -1 if not found
int find_product(const char* name, const char* category) {
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].name, name) == 0 && strcmp(products[i].category, category) == 0) {
            return i;
        }
    }
    return -1;
}

// Adds or updates a product in the array
void add_update_product(const char* name, const char* category, int quantity, const char* expiration_date) {
    int index = find_product(name, category);
    if (index == -1) {
        if (product_count < MAX_PRODUCTS) {
            strcpy(products[product_count].name, name);
            strcpy(products[product_count].category, category);
            products[product_count].quantity = quantity;
            strcpy(products[product_count].expiration_date, expiration_date);
            product_count++;
        }
    }
    else {
        products[index].quantity += quantity;
    }
}

// Deletes a product from the array
void delete_product(const char* name, const char* category) {
    int index = find_product(name, category);
    if (index != -1) {
        for (int i = index; i < product_count - 1; i++) {
            products[i] = products[i + 1];
        }
        product_count--;
    }
}

// Comparator for sorting products by quantity (ascending)
int product_comparator(const void* a, const void* b) {
    return ((Product*)a)->quantity - ((Product*)b)->quantity;
}

// Lists products containing a specific string or all if string is empty, sorted by quantity
void list_products(const char* search_str) {
    qsort(products, product_count, sizeof(Product), product_comparator);
    for (int i = 0; i < product_count; i++) {
        if (strstr(products[i].name, search_str) != NULL || strlen(search_str) == 0) {
            printf("Name: %s, Category: %s, Quantity: %d, Expiration Date: %s\n",
                products[i].name, products[i].category, products[i].quantity, products[i].expiration_date);
        }
    }
}

// Displays the menu of options to the user
static void display_options() {
    printf("\nIntelligent Refrigerator Management System\n");
    printf("1. Add/Update Product\n");
    printf("2. Delete Product\n");
    printf("3. List Products\n");
    printf("4. Exit\n");
    printf("Choose an option: ");
}

// Handles user interface interactions
void ui() {
    char search_str[100];
    char name[100];
    char category[20];
    int quantity;
    char expiration_date[20];
    int option;

    do {
        display_options();
        scanf("%d", &option);
        getchar(); // consume newline

        switch (option) {
        case 1:
            printf("Enter product name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Enter category (dairy, sweets, meat, fruit): ");
            fgets(category, sizeof(category), stdin);
            category[strcspn(category, "\n")] = 0;

            printf("Enter quantity: ");
            scanf("%d", &quantity);
            getchar();

            printf("Enter expiration date (YYYY-MM-DD): ");
            fgets(expiration_date, sizeof(expiration_date), stdin);
            expiration_date[strcspn(expiration_date, "\n")] = 0;

            add_update_product(name, category, quantity, expiration_date);
            break;

        case 2:
            printf("Enter product name to delete: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Enter category: ");
            fgets(category, sizeof(category), stdin);
            category[strcspn(category, "\n")] = 0;

            delete_product(name, category);
            break;

        case 3:
            printf("Enter search string (leave empty for all products): ");
            fgets(search_str, sizeof(search_str), stdin);
            search_str[strcspn(search_str, "\n")] = 0;

            list_products(search_str);
            break;

        case 4:
            printf("Exiting...\n");
            break;

        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (option != 4);
}

int main() {
    add_update_product("Milk", "dairy", 5, "2025-03-20");
    add_update_product("Chocolate", "sweets", 10, "2025-06-15");
    add_update_product("Chicken", "meat", 3, "2025-03-25");
    add_update_product("Apple", "fruit", 12, "2025-04-10");
    add_update_product("Milk", "dairy", 2, "2025-03-20");

    ui(); // Start the user interface
    return 0;
}
