#include "domain.h"

School::School(std::string name, Address address, Date date)
{
	this->name = name;
	this->adress = address;
	this->date = date;
	this->visited = false;
}
std::string School::GetName()
{
	return this->name;
}

Address School::GetAddress()
{
	return this->adress;
}

Date School::GetDate()
{
	return this->date;
}

bool School::IsVisited() const
{
	return this->visited;
}

void School::SetVisited(bool value)
{
	this->visited = value;
}




