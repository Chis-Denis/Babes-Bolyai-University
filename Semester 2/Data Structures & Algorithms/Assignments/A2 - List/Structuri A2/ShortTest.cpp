#include <assert.h>
#include <exception>
#include <iostream>

#include "ShortTest.h"
#include "IndexedList.h"
#include "ListIterator.h"




void testAll() {
    IndexedList list = IndexedList();
    assert(list.isEmpty());
    list.addToEnd(1);
    assert(list.size() == 1);
    list.addToPosition(0,2);
    assert(list.size() == 2);
    ListIterator it = list.iterator();
    assert(it.valid());
    it.next();
    assert(it.getCurrent() == 1);
    it.first();
    assert(it.getCurrent() == 2);
    assert(list.search(1) == 1);
    assert(list.setElement(1,3) == 1);
    assert(list.getElement(1) == 3);
    assert(list.remove(0) == 2);
    assert(list.size() == 1);
     
}

//make some tests so it can test the functionality of removeBetween method
//make some tests that add 5 elements to the list and then remove the elements from position 1 to 3; and adds some more things in order to test the functionality
void testFunction()
{
    IndexedList list = IndexedList();
    ListIterator it = list.iterator();
    assert(it.valid() == false);
    list.addToEnd(1);
    list.addToEnd(2);
    list.addToEnd(3);
    list.addToEnd(4);
    list.addToEnd(5);
    assert(list.size() == 5);
    list.removeBetween(1, 3);
    assert(list.size() == 2);
    assert(list.getElement(0) == 1);
    assert(list.getElement(1) == 5);  

    std::cout << std::endl;
    std::cout << "All tests passed!" << std::endl;
}