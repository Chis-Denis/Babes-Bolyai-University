#pragma once
#include "Service.h"
class UI
{
private:
	Service service;
public:
	UI();

	void RemoveSchoolUI();
	void DisplayAllUI();
	void start();
	void ShowSchoolsToVisitUI();
};

