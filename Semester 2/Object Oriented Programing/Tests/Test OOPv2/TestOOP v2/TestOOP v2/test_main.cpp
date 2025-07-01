#include "repository.h"
#include <iostream>

int main() {
    std::cout << "Starting repository tests..." << std::endl;
    
    // Create a repository
    Repository repo;
    
    // Run the tests
    bool removeTestResult = repo.test_remove();
    bool dateTestResult = repo.test_GetSchoolsToVisitAfterDate();
    
    // Report results
    if (removeTestResult && dateTestResult) {
        std::cout << "\nAll repository tests passed successfully!" << std::endl;
    } else {
        std::cout << "\nSome repository tests failed!" << std::endl;
    }
    
    return 0;
} 