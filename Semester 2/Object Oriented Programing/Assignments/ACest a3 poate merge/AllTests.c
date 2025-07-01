#include "AllTests.h"
#include "product_test.h"
#include "dynamic_array_test.h"
#include <stdio.h>

// Runs all test suites in the application
void run_all_tests() {
    printf("Running all tests...\n\n");
    
    printf("Running product tests...\n");
    run_all_product_tests();
    
    printf("Running dynamic array tests...\n");
    run_all_dynamic_array_tests();
    
    printf("Running service tests...\n");
    run_all_service_tests();
    
    printf("All tests completed successfully!\n\n");
} 