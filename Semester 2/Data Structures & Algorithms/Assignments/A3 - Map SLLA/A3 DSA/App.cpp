#include "Set.h"
#include "SetIterator.h"
#include "ExtendedTest.h"
#include "ShortTest.h"
#include <stack>
#include <iostream>
#include "TestIterator.h"
using namespace std;




int main() {

	testAll();
	testAllExtended();
	testIteratorPlus();

	cout << "That's all!" << endl;
	system("pause");

}

//we will create a doubly linked list on an array using an array for the values and another array for the next and previous elements



