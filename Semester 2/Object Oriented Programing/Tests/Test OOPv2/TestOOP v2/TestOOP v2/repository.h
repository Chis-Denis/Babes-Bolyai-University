#pragma once
#include "domain.h"
#include "Dynamic_vector.h"
#include <string>
#include <cassert>
#include <iostream>

class Repository
{
private:
	DynamicVector<School> elements;
public:
	Repository();

	void InitialSchoolsRepo();

	/**
	 * Removes a school from the repository.
	 * @return true if the school was removed, false otherwise
	 */
	bool RemoveSchoolRepo(std::string name_inputed, Address address_inputed);
	DynamicVector<School> DisplayAllRepo();

	/**
	 * Gets schools that need to be visited after a given date.
	 * @return A DynamicVector containing schools to visit after the given date
	 */
	DynamicVector<School> GetSchoolsToVisitAfterDate(Date date);
	bool test_remove();
	bool test_GetSchoolsToVisitAfterDate();
};

