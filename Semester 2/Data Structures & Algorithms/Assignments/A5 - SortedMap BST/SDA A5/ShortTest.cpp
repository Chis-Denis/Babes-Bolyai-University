#include <assert.h>

#include "SortedMap.h"
#include "SMIterator.h"
#include "ShortTest.h"
#include <exception>
using namespace std;

bool relatie1(TKey cheie1, TKey cheie2) {
	if (cheie1 <= cheie2) {
		return true;
	}
	else {
		return false;
	}
}

void testAll(){
	SortedMap sm(relatie1);
	assert(sm.size() == 0);
	assert(sm.isEmpty());
    sm.add(1,2);
    assert(sm.size() == 1);
    assert(!sm.isEmpty());
    assert(sm.search(1)!=NULL_TVALUE);
    TValue v =sm.add(1,3);
    assert(v == 2);
    assert(sm.search(1) == 3);
    SMIterator it = sm.iterator();
    it.first();
    while (it.valid()){
    	TElem e = it.getCurrent();
    	assert(e.second != NULL_TVALUE);
    	it.next();
    }
    assert(sm.remove(1) == 3);
    assert(sm.isEmpty());
}

//it tests the getKeyRange function
void testExtra()
{
    SortedMap sm(relatie1);
	assert(sm.getKeyRange() == -1);
	sm.add(1, 2);
	assert(sm.getKeyRange() == 0);
	sm.add(2, 3);
	assert(sm.getKeyRange() == 1);
	sm.add(3, 4);
	assert(sm.getKeyRange() == 2);
	sm.add(4, 5);
	assert(sm.getKeyRange() == 3);
	sm.add(5, 6);
	assert(sm.getKeyRange() == 4);
	sm.add(6, 7);
	assert(sm.getKeyRange() == 5);
}

