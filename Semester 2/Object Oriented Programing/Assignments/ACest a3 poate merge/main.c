#include "user_interface.h"
#include "AllTests.h"
#include <stdio.h>

int main() {
    // Run all tests first
    run_all_tests();
    
    // Create the application layers
    Repository* repo = create_repository();
    if (repo == NULL) {
        printf("Error: Failed to create repository!\n");
        return 1;
    }
    
    
    ProductService* service = create_service(repo);
    if (service == NULL) {
        printf("Error: Failed to create service!\n");
        destroy_repository(repo);
        return 1;
    }
    
    UserInterface* ui = create_ui(service);
    if (ui == NULL) {
        printf("Error: Failed to create UI!\n");
        destroy_service(service);
        destroy_repository(repo);
        return 1;
    }
    
    // Run the application
    printf("Starting Smart Refrigerator Application...\n\n");
    run_ui(ui);
    
    // Cleanup
    destroy_ui(ui);
    destroy_service(service);
    destroy_repository(repo);
    
    return 0;
}
