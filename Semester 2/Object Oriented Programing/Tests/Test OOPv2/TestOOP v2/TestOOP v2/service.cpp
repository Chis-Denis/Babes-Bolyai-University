#include "service.h"
#include "string"

Service::Service()
{

}

//Removes a school from the service return true if the school was removed, false otherwise
bool Service::RemoveSchoolService(std::string name_inputed, Address address_inputed)
{
	return this->repo.RemoveSchoolRepo(name_inputed, address_inputed);
}

DynamicVector<School> Service::DisplayAllService()
{
	return this->repo.DisplayAllRepo();
}

//Gets schools that need to be visited after a given date return A DynamicVector containing schools to visit after the given date
DynamicVector<School> Service::GetSchoolsToVisitAfterDate(Date date)
{
	return repo.GetSchoolsToVisitAfterDate(date);
}