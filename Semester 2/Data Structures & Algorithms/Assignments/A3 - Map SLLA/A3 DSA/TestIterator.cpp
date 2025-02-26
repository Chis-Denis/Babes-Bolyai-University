#include "TestIterator.h"
#include "Set.h"
#include "SetIterator.h"

#include <cassert>
#include <iostream>

void testIteratorPlus() 
{
    // Create a set and add some elements
    Set set;
    set.add(1);
    set.add(2);
    set.add(3);
    set.add(4);

    // Create an iterator for the set
    SetIterator it = set.iterator();

    // Test kStepsPrev function
    it.first();
    it.next();
    it.next();
    it.kStepsPrev(2);
    assert(it.getCurrent() == 1);

    it.next();
    it.next();
    it.next();
    it.kStepsPrev(3);
    assert(it.getCurrent() == 1);

    it.first();
    it.next();
    it.next();
    it.kStepsPrev(1);
    assert(it.getCurrent() == 2);

    it.first();
    it.next();
    it.next();
    it.next();
    it.kStepsPrev(1);
    it.kStepsPrev(1);
    assert(it.getCurrent() == 2);

    it.first();
    try {
		it.kStepsPrev(1);
		assert(false);
	}
    catch (std::exception& ex) {
		assert(true);
	}

    std::cout << "All SetIterator tests passed!" << std::endl;
}
