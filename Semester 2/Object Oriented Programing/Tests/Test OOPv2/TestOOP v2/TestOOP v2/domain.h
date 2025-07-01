#pragma once
#include <string>
struct Address {
	double longitude;
	double latitude;
};
struct Date {
	int day;
	int month;
	int year;
};
class School
{
private:
	std::string name;
	Address adress;
	Date date;
	bool visited;
public:

	School(std::string name, Address adress, Date date);
	School() : name(""), adress({ 0,0 }), date({ 0,0,0 }), visited(false){};
	std::string GetName();
	Address GetAddress();
	Date GetDate();
	bool IsVisited() const;
	void SetVisited(bool value);
};

