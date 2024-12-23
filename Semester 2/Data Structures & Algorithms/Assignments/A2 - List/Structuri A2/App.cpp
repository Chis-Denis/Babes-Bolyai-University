#include <iostream>
#include "ShortTest.h"
#include "ExtendedTest.h"

using namespace std;

int main(){
    testAll();
    testAllExtended();
    testFunction();
    cout<<"Finished LI Tests!"<<endl;
}

//Implement a double indexed linked list
//Add an extra operation remove all elements between two positions (start, end); throw an exception if start or end are invalid