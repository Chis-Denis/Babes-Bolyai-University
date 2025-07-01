#include "user_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <crtdbg.h>

#define MAX_PRODUCTS 100
#define MAX_LINE 100
#define MAX_NAME_LENGTH 49
#define MIN_QUANTITY 0.01
#define MAX_QUANTITY 1000.0
#define MIN_DAYS 1
#define MAX_DAYS 365

// Clears any remaining characters in the input buffer
static void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Reads a line of input safely, handling buffer overflow
static int read_line(char* buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) return 0;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        return 1;
    }
    
    // Clear any remaining input
    clear_input_buffer();
    return 1;
}

// Validates if a product name contains only allowed characters and proper length
static int is_valid_product_name(const char* name) {
    if (name == NULL || strlen(name) == 0 || strlen(name) > MAX_NAME_LENGTH) return 0;
    
    // Check if name contains only letters, numbers, and spaces
    for (size_t i = 0; name[i]; i++) {
        if (!isalnum(name[i]) && !isspace(name[i])) return 0;
    }
    
    // Name shouldn't start or end with space
    return !isspace(name[0]) && !isspace(name[strlen(name) - 1]);
}

// Creates a new user interface instance with the given service
UserInterface* create_ui(ProductService* service) {
    if (service == NULL) return NULL;
    
    UserInterface* ui = (UserInterface*)malloc(sizeof(UserInterface));
    if (ui == NULL) return NULL;
    
    ui->service = service;
    return ui;
}

// Frees memory used by the user interface
void destroy_ui(UserInterface* ui) {
    if (ui != NULL) {
        free(ui);
    }
}

// Displays the main menu options to the user
void print_menu() {
    printf("\nSmart Refrigerator Menu:\n");
    printf("1. Add product\n");
    printf("2. Delete product\n");
    printf("3. Update product\n");
    printf("4. Search products\n");
    printf("5. View expiring products\n");
    printf("6. Undo\n");
    printf("7. Redo\n");
    printf("0. Exit\n");
    printf("Choose an option: ");
}

// Displays a formatted list of products
void print_products(Product* products, size_t size) {
    printf("\n%-20s %-10s %-10s %s\n", "Name", "Category", "Quantity", "Expiration Date");
    printf("%s\n", "------------------------------------------------");
    
    for (size_t i = 0; i < size; i++) {
        char product_str[100];
        product_to_string(&products[i], product_str, sizeof(product_str));
        printf("%s\n", product_str);
    }
    printf("\n");
}

// Prompts user to select a product category
Category get_category() {
    printf("\nChoose category:\n");
    printf("1. Dairy\n");
    printf("2. Sweets\n");
    printf("3. Meat\n");
    printf("4. Fruit\n");
    
    int choice;
    char input[10];
    while (1) {
        printf("Enter choice (1-4): ");
        if (!read_line(input, sizeof(input))) continue;
        
        // Check if input is a single digit
        if (strlen(input) != 1 || !isdigit(input[0])) {
            printf("Invalid input! Please enter a single digit (1-4).\n");
            continue;
        }
        
        choice = input[0] - '0';
        if (choice >= 1 && choice <= 4) {
            return (Category)(choice - 1);
        }
        printf("Invalid choice! Please enter a number between 1 and 4.\n");
    }
}

// Gets and validates a quantity value from user input
static double get_quantity(const char* prompt) {
    char input[20];
    double quantity;
    
    while (1) {
        printf("%s (%.2f-%.2f): ", prompt, MIN_QUANTITY, MAX_QUANTITY);
        if (!read_line(input, sizeof(input))) continue;
        
        // Check if input contains only digits, decimal point, and optional negative sign
        int valid = 1;
        int decimal_points = 0;
        for (size_t i = 0; input[i]; i++) {
            if (i == 0 && input[i] == '-') continue;
            if (input[i] == '.') {
                decimal_points++;
                if (decimal_points > 1) {
                    valid = 0;
                    break;
                }
                continue;
            }
            if (!isdigit(input[i])) {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            printf("Invalid input! Please enter a valid number.\n");
            continue;
        }
        
        quantity = atof(input);
        if (quantity >= MIN_QUANTITY && quantity <= MAX_QUANTITY) {
            return quantity;
        }
        
        printf("Invalid quantity! Please enter a number between %.2f and %.2f.\n", 
               MIN_QUANTITY, MAX_QUANTITY);
    }
}

// Gets and validates a number of days from user input
static int get_days(const char* prompt) {
    char input[10];
    int days;
    
    while (1) {
        printf("%s (%d-%d): ", prompt, MIN_DAYS, MAX_DAYS);
        if (!read_line(input, sizeof(input))) continue;
        
        // Check if input contains only digits
        int valid = 1;
        for (size_t i = 0; input[i]; i++) {
            if (!isdigit(input[i])) {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            printf("Invalid input! Please enter a valid number.\n");
            continue;
        }
        
        days = atoi(input);
        if (days >= MIN_DAYS && days <= MAX_DAYS) {
            return days;
        }
        
        printf("Invalid number of days! Please enter a number between %d and %d.\n", 
               MIN_DAYS, MAX_DAYS);
    }
}

// Gets and validates a product name from user input
static void get_product_name(char* name, size_t size) {
    while (1) {
        printf("Enter product name: ");
        if (!read_line(name, size)) {
            printf("Error reading input! Please try again.\n");
            continue;
        }
        
        if (is_valid_product_name(name)) {
            return;
        }
        
        printf("Invalid product name! Name should:\n");
        printf("- Be between 1 and %d characters\n", MAX_NAME_LENGTH);
        printf("- Contain only letters, numbers, and spaces\n");
        printf("- Not start or end with spaces\n");
    }
}

// Handles the add product menu option
void handle_add_product(UserInterface* ui) {
    char name[50];
    get_product_name(name, sizeof(name));
    
    Category category = get_category();
    double quantity = get_quantity("Enter quantity");
    int days = get_days("Enter days until expiration");
    
    time_t expiration = time(NULL) + days * 86400;
    
    if (service_add_product(ui->service, name, category, quantity, expiration)) {
        printf("\nProduct added successfully!\n");
    } else {
        printf("\nFailed to add product! Please try again.\n");
    }
}

// Handles the delete product menu option
void handle_delete_product(UserInterface* ui) {
    char name[50];
    get_product_name(name, sizeof(name));
    
    Category category = get_category();
    
    if (service_delete_product(ui->service, name, category)) {
        printf("\nProduct deleted successfully!\n");
    } else {
        printf("\nProduct not found!\n");
    }
}

// Handles the update product menu option
void handle_update_product(UserInterface* ui) {
    char name[50];
    get_product_name(name, sizeof(name));
    
    Category category = get_category();
    double quantity = get_quantity("Enter new quantity");
    int days = get_days("Enter new days until expiration");
    
    time_t expiration = time(NULL) + days * 86400;
    
    if (service_update_product(ui->service, name, category, quantity, expiration)) {
        printf("\nProduct updated successfully!\n");
    } else {
        printf("\nProduct not found!\n");
    }
}

// Handles the search products menu option
void handle_search_products(UserInterface* ui) {
    char search_string[50];
    Product result[MAX_PRODUCTS];
    size_t result_size;
    
    printf("Enter search string (empty for all products): ");
    if (!read_line(search_string, sizeof(search_string))) {
        printf("Error reading input! Please try again.\n");
        return;
    }
    
    // Validate search string (allow empty string)
    if (strlen(search_string) > 0 && !is_valid_product_name(search_string)) {
        printf("Invalid search string! Please use only letters, numbers, and spaces.\n");
        return;
    }
    
    service_get_products_by_name(ui->service, search_string, result, &result_size);
    
    if (result_size > 0) {
        print_products(result, result_size);
    } else {
        printf("No products found!\n");
    }
}

// Handles the expiring products menu option
void handle_expiring_products(UserInterface* ui) {
    Product result[MAX_PRODUCTS];
    size_t result_size;
    
    int days = get_days("Enter number of days to check");
    
    printf("\nChoose category:\n");
    printf("0. All categories\n");
    printf("1. Dairy\n");
    printf("2. Sweets\n");
    printf("3. Meat\n");
    printf("4. Fruit\n");
    
    char input[10];
    int choice;
    Category category;
    
    while (1) {
        printf("Enter choice (0-4): ");
        if (!read_line(input, sizeof(input))) continue;
        
        // Check if input is a single digit
        if (strlen(input) != 1 || !isdigit(input[0])) {
            printf("Invalid input! Please enter a single digit (0-4).\n");
            continue;
        }
        
        choice = input[0] - '0';
        if (choice >= 0 && choice <= 4) {
            category = (choice == 0) ? CATEGORY_ANY : (Category)(choice - 1);
            break;
        }
        printf("Invalid choice! Please enter a number between 0 and 4.\n");
    }
    
    service_get_expiring_products(ui->service, category, days, result, &result_size);
    
    if (result_size > 0) {
        print_products(result, result_size);
    } else {
        printf("No expiring products found!\n");
    }
}

// Main UI loop that handles user interaction
void run_ui(UserInterface* ui) {
    if (!service_init_default_products(ui->service)) {
        printf("Warning: Failed to initialize default products!\n\n");
    }
    
    int running = 1;
    while (running) {
        print_menu();
        
        char input[10];
        if (!read_line(input, sizeof(input))) {
            printf("Error reading input! Please try again.\n");
            continue;
        }
        
        // Check if input is a single digit
        if (strlen(input) != 1 || !isdigit(input[0])) {
            printf("Invalid input! Please enter a single digit (0-7).\n");
            continue;
        }
        
        int choice = input[0] - '0';
        
        switch (choice) {
            case 1:
                handle_add_product(ui);
                break;
            case 2:
                handle_delete_product(ui);
                break;
            case 3:
                handle_update_product(ui);
                break;
            case 4:
                handle_search_products(ui);
                break;
            case 5:
                handle_expiring_products(ui);
                break;
            case 6:
                if (service_undo(ui->service)) {
                    printf("\nUndo successful!\n");
                } else {
                    printf("\nNothing to undo!\n");
                }
                break;
            case 7:
                if (service_redo(ui->service)) {
                    printf("\nRedo successful!\n");
                } else {
                    printf("\nNothing to redo!\n");
                }
                break;
            case 0:
                running = 0;
                printf("\nThank you for using Smart Refrigerator!\n");
                _CrtDumpMemoryLeaks();
                break;
            default:
                printf("\nInvalid option! Please enter a number between 0 and 7.\n");
        }
    }
} 