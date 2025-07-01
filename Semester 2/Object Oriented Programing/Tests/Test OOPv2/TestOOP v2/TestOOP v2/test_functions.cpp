#include <iostream>
#include <cassert>
#include "repository.h"
#include "service.h"

// Test function for Repository::RemoveSchoolRepo
void testRemoveSchoolRepo() {
    std::cout << "Testing Repository::RemoveSchoolRepo..." << std::endl;
    
    // Create a repository
    Repository repo;
    
    // Get initial size
    int initialSize = repo.DisplayAllRepo().getSize();
    std::cout << "Initial repository size: " << initialSize << std::endl;
    
    // Test case 1: Remove an existing school
    Address address = {5.2, 6.0}; // Address of "Mircea" school
    bool result = repo.RemoveSchoolRepo("Mircea", address);
    assert(result == true);
    std::cout << "Test case 1 passed: Successfully removed existing school" << std::endl;
    
    // Verify the school was removed
    int newSize = repo.DisplayAllRepo().getSize();
    assert(newSize == initialSize - 1);
    std::cout << "Repository size after removal: " << newSize << std::endl;
    
    // Test case 2: Try to remove a non-existent school
    result = repo.RemoveSchoolRepo("NonExistentSchool", {0.0, 0.0});
    assert(result == false);
    std::cout << "Test case 2 passed: Failed to remove non-existent school" << std::endl;
    
    // Verify the size didn't change
    int finalSize = repo.DisplayAllRepo().getSize();
    assert(finalSize == newSize);
    std::cout << "Repository size after failed removal: " << finalSize << std::endl;
    
    std::cout << "All Repository::RemoveSchoolRepo tests passed!" << std::endl;
}

// Test function for Service::RemoveSchoolService
void testRemoveSchoolService() {
    std::cout << "\nTesting Service::RemoveSchoolService..." << std::endl;
    
    // Create a service
    Service service;
    
    // Get initial size
    int initialSize = service.DisplayAllService().getSize();
    std::cout << "Initial service size: " << initialSize << std::endl;
    
    // Test case 1: Remove an existing school
    Address address = {5.3, 6.1}; // Address of "Alehandru" school
    bool result = service.RemoveSchoolService("Alehandru", address);
    assert(result == true);
    std::cout << "Test case 1 passed: Successfully removed existing school" << std::endl;
    
    // Verify the school was removed
    int newSize = service.DisplayAllService().getSize();
    assert(newSize == initialSize - 1);
    std::cout << "Service size after removal: " << newSize << std::endl;
    
    // Test case 2: Try to remove a non-existent school
    result = service.RemoveSchoolService("NonExistentSchool", {0.0, 0.0});
    assert(result == false);
    std::cout << "Test case 2 passed: Failed to remove non-existent school" << std::endl;
    
    // Verify the size didn't change
    int finalSize = service.DisplayAllService().getSize();
    assert(finalSize == newSize);
    std::cout << "Service size after failed removal: " << finalSize << std::endl;
    
    std::cout << "All Service::RemoveSchoolService tests passed!" << std::endl;
}

// Test function for Repository::GetSchoolsToVisitAfterDate
void testGetSchoolsToVisitAfterDate() {
    std::cout << "\nTesting Repository::GetSchoolsToVisitAfterDate..." << std::endl;
    
    // Create a repository
    Repository repo;
    
    // Test case 1: Get schools to visit after a date in the past
    Date pastDate = {1, 1, 2020}; // January 1, 2020
    DynamicVector<School> schoolsToVisit = repo.GetSchoolsToVisitAfterDate(pastDate);
    
    // Count schools with dates after pastDate
    int expectedCount = 0;
    DynamicVector<School> allSchools = repo.DisplayAllRepo();
    for (int i = 0; i < allSchools.getSize(); i++) {
        School s = allSchools.getElemFromPosition(i);
        Date schoolDate = s.GetDate();
        
        if (schoolDate.year > pastDate.year || 
            (schoolDate.year == pastDate.year && schoolDate.month > pastDate.month) ||
            (schoolDate.year == pastDate.year && schoolDate.month == pastDate.month && schoolDate.day > pastDate.day)) {
            expectedCount++;
        }
    }
    
    assert(schoolsToVisit.getSize() == expectedCount);
    std::cout << "Test case 1 passed: Found " << schoolsToVisit.getSize() << " schools to visit after " 
              << pastDate.year << "." << pastDate.month << "." << pastDate.day << std::endl;
    
    // Test case 2: Get schools to visit after a date in the future
    Date futureDate = {1, 1, 2025}; // January 1, 2025
    schoolsToVisit = repo.GetSchoolsToVisitAfterDate(futureDate);
    
    // Count schools with dates after futureDate
    expectedCount = 0;
    allSchools = repo.DisplayAllRepo();
    for (int i = 0; i < allSchools.getSize(); i++) {
        School s = allSchools.getElemFromPosition(i);
        Date schoolDate = s.GetDate();
        
        if (schoolDate.year > futureDate.year || 
            (schoolDate.year == futureDate.year && schoolDate.month > futureDate.month) ||
            (schoolDate.year == futureDate.year && schoolDate.month == futureDate.month && schoolDate.day > futureDate.day)) {
            expectedCount++;
        }
    }
    
    assert(schoolsToVisit.getSize() == expectedCount);
    std::cout << "Test case 2 passed: Found " << schoolsToVisit.getSize() << " schools to visit after " 
              << futureDate.year << "." << futureDate.month << "." << futureDate.day << std::endl;
    
    // Test case 3: Verify that schools with dates before or equal to the given date are marked as visited
    Date checkDate = {1, 1, 2021}; // January 1, 2021
    repo.GetSchoolsToVisitAfterDate(checkDate);
    
    allSchools = repo.DisplayAllRepo();
    for (int i = 0; i < allSchools.getSize(); i++) {
        School s = allSchools.getElemFromPosition(i);
        Date schoolDate = s.GetDate();
        
        if (schoolDate.year < checkDate.year || 
            (schoolDate.year == checkDate.year && schoolDate.month < checkDate.month) ||
            (schoolDate.year == checkDate.year && schoolDate.month == checkDate.month && schoolDate.day <= checkDate.day)) {
            assert(s.IsVisited() == true);
        }
    }
    std::cout << "Test case 3 passed: Schools with dates before or equal to " 
              << checkDate.year << "." << checkDate.month << "." << checkDate.day 
              << " are marked as visited" << std::endl;
    
    std::cout << "All Repository::GetSchoolsToVisitAfterDate tests passed!" << std::endl;
}

// Test function for Service::GetSchoolsToVisitAfterDate
void testGetSchoolsToVisitAfterDateService() {
    std::cout << "\nTesting Service::GetSchoolsToVisitAfterDate..." << std::endl;
    
    // Create a service
    Service service;
    
    // Test case 1: Get schools to visit after a date in the past
    Date pastDate = {1, 1, 2020}; // January 1, 2020
    DynamicVector<School> schoolsToVisit = service.GetSchoolsToVisitAfterDate(pastDate);
    
    // Count schools with dates after pastDate
    int expectedCount = 0;
    DynamicVector<School> allSchools = service.DisplayAllService();
    for (int i = 0; i < allSchools.getSize(); i++) {
        School s = allSchools.getElemFromPosition(i);
        Date schoolDate = s.GetDate();
        
        if (schoolDate.year > pastDate.year || 
            (schoolDate.year == pastDate.year && schoolDate.month > pastDate.month) ||
            (schoolDate.year == pastDate.year && schoolDate.month == pastDate.month && schoolDate.day > pastDate.day)) {
            expectedCount++;
        }
    }
    
    assert(schoolsToVisit.getSize() == expectedCount);
    std::cout << "Test case 1 passed: Found " << schoolsToVisit.getSize() << " schools to visit after " 
              << pastDate.year << "." << pastDate.month << "." << pastDate.day << std::endl;
    
    // Test case 2: Get schools to visit after a date in the future
    Date futureDate = {1, 1, 2025}; // January 1, 2025
    schoolsToVisit = service.GetSchoolsToVisitAfterDate(futureDate);
    
    // Count schools with dates after futureDate
    expectedCount = 0;
    allSchools = service.DisplayAllService();
    for (int i = 0; i < allSchools.getSize(); i++) {
        School s = allSchools.getElemFromPosition(i);
        Date schoolDate = s.GetDate();
        
        if (schoolDate.year > futureDate.year || 
            (schoolDate.year == futureDate.year && schoolDate.month > futureDate.month) ||
            (schoolDate.year == futureDate.year && schoolDate.month == futureDate.month && schoolDate.day > futureDate.day)) {
            expectedCount++;
        }
    }
    
    assert(schoolsToVisit.getSize() == expectedCount);
    std::cout << "Test case 2 passed: Found " << schoolsToVisit.getSize() << " schools to visit after " 
              << futureDate.year << "." << futureDate.month << "." << futureDate.day << std::endl;
    
    std::cout << "All Service::GetSchoolsToVisitAfterDate tests passed!" << std::endl;
}

int main() {    
    std::cout << "Starting tests..." << std::endl;
    
    // Run all tests
    testRemoveSchoolRepo();
    testRemoveSchoolService();
    testGetSchoolsToVisitAfterDate();
    testGetSchoolsToVisitAfterDateService();
    
    std::cout << "\nAll tests completed successfully!" << std::endl;
    return 0;
} 