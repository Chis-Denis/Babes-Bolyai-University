#pragma once
#include "Repository.h"
#include <string>
#include "dynamic_vector.h"
class Service
{
private:
	Repository repo;

public:
	Service();

	/**
	 * Removes a school from the service.
	 * @return true if the school was removed, false otherwise
	 */
	bool RemoveSchoolService(std::string name_inputed, Address address_inputed);
	DynamicVector<School> DisplayAllService();

	/**
	 * Gets schools that need to be visited after a given date.
	 * @return A DynamicVector containing schools to visit after the given date
	 */
	DynamicVector<School> GetSchoolsToVisitAfterDate(Date date);
};

