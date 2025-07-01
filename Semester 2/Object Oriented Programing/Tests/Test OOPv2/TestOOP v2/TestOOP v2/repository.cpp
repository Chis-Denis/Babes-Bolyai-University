#include "repository.h"
#include <iostream>
#include <cassert>

Repository::Repository()
{
	this->InitialSchoolsRepo();
}

void Repository::InitialSchoolsRepo()
{
    
    Address a1, a2, a3, a4, a5;

    a1.latitude = 5.2;
    a1.longitude = 6.0;

    a2.latitude = 5.3;
    a2.longitude = 6.1;

    a3.latitude = 5.4;
    a3.longitude = 6.2;

    a4.latitude = 5.5;
    a4.longitude = 6.3;

    a5.latitude = 5.6;
    a5.longitude = 6.4;

    Date d1, d2, d3, d4, d5;

    d1.day = 31;
    d1.month = 6;
    d1.year = 2019;

    d2.day = 17;
    d2.month = 1;
    d2.year = 2020;

    d3.day = 3;
    d3.month = 8;
    d3.year = 2024;

    d4.day = 89;
    d4.month = 1;
    d4.year = 2022;

    d5.day = 5;
    d5.month = 1;
    d5.year = 2020;

    School s1("Mircea", a1, d1);
    School s2("Alehandru", a2, d2);
    School s3("Coboi", a3, d3);
    School s4("Manea", a4, d4);
    School s5("Contabilu", a5, d5);

    this->elements.addElemToVector(s1);
    this->elements.addElemToVector(s2);
    this->elements.addElemToVector(s3);
    this->elements.addElemToVector(s4);
    this->elements.addElemToVector(s5);

}

//Removes a school from the repository return true if the school was removed, false otherwise
bool Repository::RemoveSchoolRepo(std::string name_inputed, Address address_inputed)
{
	for (int i = 0; i < elements.getSize(); i++)
	{
		School currentSchool = elements.getElemFromPosition(i);
		if (currentSchool.GetName() == name_inputed && 
			currentSchool.GetAddress().longitude == address_inputed.longitude && 
			currentSchool.GetAddress().latitude == address_inputed.latitude)
		{
			elements.deleteElemFromVector(i);
			return true;
		}
	}
	return false;
}

DynamicVector<School> Repository::DisplayAllRepo()
{
    DynamicVector<School> sortedSchools = this->elements;
    int size = sortedSchools.getSize();
    
    // Bubble sort by school name
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (sortedSchools.getElemFromPosition(j).GetName() > sortedSchools.getElemFromPosition(j + 1).GetName()) {
                // Swap elements
                School temp = sortedSchools.getElemFromPosition(j);
                sortedSchools.setElemAtPosition(j, sortedSchools.getElemFromPosition(j + 1));
                sortedSchools.setElemAtPosition(j + 1, temp);
            }
        }
    }
    
    return sortedSchools;
}

//Gets schools that need to be visited after a given date return A DynamicVector containing schools to visit after the given date
DynamicVector<School> Repository::GetSchoolsToVisitAfterDate(Date date)
{
    DynamicVector<School> schoolsToVisit;
    for (int i = 0; i < elements.getSize(); i++)
    {
        School currentSchool = elements.getElemFromPosition(i);
        Date schoolDate = currentSchool.GetDate();
        
        // Check if school date is before or equal to the given date
        if (schoolDate.year < date.year || 
            (schoolDate.year == date.year && schoolDate.month < date.month) ||
            (schoolDate.year == date.year && schoolDate.month == date.month && schoolDate.day <= date.day))
        {
            // Mark as visited
            currentSchool.SetVisited(true);
            elements.setElemAtPosition(i, currentSchool);
        }
        // If school date is after the given date, add to schools to visit
        else if (schoolDate.year > date.year || 
            (schoolDate.year == date.year && schoolDate.month > date.month) ||
            (schoolDate.year == date.year && schoolDate.month == date.month && schoolDate.day > date.day))
        {
            schoolsToVisit.addElemToVector(currentSchool);
        }
    }
    return schoolsToVisit;
}

bool Repository::test_remove() {
    std::cout << "Testing Repository::RemoveSchoolRepo..." << std::endl;
    
    // Get initial size
    int initialSize = this->DisplayAllRepo().getSize();
    std::cout << "Initial repository size: " << initialSize << std::endl;
    
    // Test case 1: Remove an existing school
    Address address = {5.2, 6.0};
    bool result = this->RemoveSchoolRepo("Mircea", address);
    assert(result == true);
    std::cout << "Test case 1 passed: Successfully removed existing school" << std::endl;
    
    // Verify the school was removed
    int newSize = this->DisplayAllRepo().getSize();
    assert(newSize == initialSize - 1);
    std::cout << "Repository size after removal: " << newSize << std::endl;
    
    std::cout << "All Repository::RemoveSchoolRepo tests passed!" << std::endl;
    return true;
}

bool Repository::test_GetSchoolsToVisitAfterDate() {
    std::cout << "Testing Repository::GetSchoolsToVisitAfterDate..." << std::endl;
    
    //Get schools to visit after a date in the past
    Date pastDate = {1, 1, 2020};
    DynamicVector<School> schoolsToVisit = this->GetSchoolsToVisitAfterDate(pastDate);
    
    // Count schools with dates after pastDate
    int expectedCount = 0;
    DynamicVector<School> allSchools = this->DisplayAllRepo();
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
    
    std::cout << "All Repository::GetSchoolsToVisitAfterDate tests passed!" << std::endl;
    return true;
}