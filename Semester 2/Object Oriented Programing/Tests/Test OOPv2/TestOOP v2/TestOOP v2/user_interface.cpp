#include "user_interface.h"
#include <string>
#include <iostream>

UI::UI()
{

}

void UI::RemoveSchoolUI()
{
	std::string name_inputed;
	Address address_inputed;

	std::cout << "Enter school name: ";
	std::cin.ignore();
	std::getline(std::cin, name_inputed);
	
	std::cout << "Enter school address (latitude,longitude): ";
	std::cin >> address_inputed.latitude >> address_inputed.longitude;

	bool success = this->service.RemoveSchoolService(name_inputed, address_inputed);

	if (success)
		std::cout << "School was removed successfully!\n";
	else
		std::cout << "School with the given name and address does not exist!\n";
}

void UI::DisplayAllUI()
{
	DynamicVector<School> schools = this->service.DisplayAllService();
	for (int i = 0; i < schools.getSize(); i++)
	{
		School s = schools.getElemFromPosition(i);
		std::cout << s.GetName() << "|" << s.GetAddress().latitude << "," << s.GetAddress().longitude << "|" << s.GetDate().day << "." << s.GetDate().month << "." << s.GetDate().year << "|" << (s.IsVisited() ? "Visited" : "Not Visited") << "\n";
	}
}

void UI::ShowSchoolsToVisitUI()
{
	Date inputDate;
	std::cout << "Enter date (yyyy.mm.dd): ";
	std::cin >> inputDate.year >> inputDate.month >> inputDate.day;
	
	DynamicVector<School> schoolsToVisit = this->service.GetSchoolsToVisitAfterDate(inputDate);
	
	if (schoolsToVisit.getSize() == 0) {
		std::cout << "No schools need to be visited after the given date.\n";
		return;
	}
	
	std::cout << "Schools that need to be visited after " << inputDate.year << "." << inputDate.month << "." << inputDate.day << ":\n";
	for (int i = 0; i < schoolsToVisit.getSize(); i++)
	{
		School s = schoolsToVisit.getElemFromPosition(i);
		std::cout << s.GetName() << "|" << s.GetAddress().latitude << "," << s.GetAddress().longitude << "|" << s.GetDate().day << "." << s.GetDate().month << "." << s.GetDate().year << "|" << (s.IsVisited() ? "Visited" : "Not Visited") << "\n";
	}
}

void PrintMenu()
{
	std::cout << std::endl;
	std::cout << "1. Remove a school." << std::endl;
	std::cout << "2. Display all schools." << std::endl;
	std::cout << "3. Show schools to visit after date." << std::endl;
	std::cout << "0. Exit." << std::endl;
	std::cout << std::endl;
}

void UI::start()
{
	bool done = false;
	while (!done)
	{
		PrintMenu();
		int command;
		std::cout << "Enter command: ";
		std::cin >> command;
		if (command == 0)
			exit(0);
		else if (command == 1)
			this->RemoveSchoolUI();
		else if (command == 2)
			this->DisplayAllUI();
		else if (command == 3)
			this->ShowSchoolsToVisitUI();
	}
}