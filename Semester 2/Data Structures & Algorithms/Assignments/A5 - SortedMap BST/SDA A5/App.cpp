#include "ExtendedTest.h"
#include "ShortTest.h"

#include "SortedMap.h"


#include <iostream>
using namespace std;


int main() {
	testAll();
	cout << "Short tests passed" << endl;
	testExtra();
	cout << endl;
	cout << "The extra works \n" << endl;
	testAllExtended();

	cout << "That's all!" << endl;
	system("pause");
	return 0;
}

//implement adt sorted map using a binary search tree with linked representation with dynamic allocation
