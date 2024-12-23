#include "Map.h"
#include "MapIterator.h"
#include <exception>
#include <iostream>

using namespace std;
#define DELETED -111112


MapIterator::MapIterator(const Map& d) : map(d)
{
	current = 0;
	currentIndex = 1;
}


void MapIterator::first() 
{
	current = 0;
	currentIndex = 1;
	while (this->valid() && map.elements[current].first == NULL_TVALUE || this->valid() && map.elements[current].first == DELETED)
		current++;
}


void MapIterator::next() 
{
	if (this->valid()) {
		current++;
		currentIndex++;
		while (this->valid() && map.elements[current].first == NULL_TVALUE || this->valid() && map.elements[current].first == DELETED)
			current++;
	}
	else
		throw exception();
}


TElem MapIterator::getCurrent()
{
	if (!this->valid())
		throw exception();
	return this->map.elements[current];
}


bool MapIterator::valid() const 
{
	return currentIndex <= map.nrElements;
}



