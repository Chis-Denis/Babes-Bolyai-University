#include "SetIterator.h"
#include "Set.h"
#include <exception>

// Complexity: theta(1)
SetIterator::SetIterator(const Set& m) : set(m)
{
	this->currentElement = 0;
}

// Complexity: theta(1)
void SetIterator::first()
{
	this->currentElement = 0;
}


// Complexity: theta(1)
void SetIterator::next() 
{
	if (this->valid())
		this->currentElement++;
	else
		throw std::exception("Invalid iterator");
}

// Complexity: theta(1)
void SetIterator::prev()
{
	if (this->valid())
		this->currentElement--;
	else
		throw std::exception("Invalid iterator");
}

// Complexity: theta(1)
TElem SetIterator::getCurrent()
{
	if (this->valid())
		return this->set.elements[this->currentElement];
	else
		throw std::exception("Invalid iterator");
}

// Complexity: theta(1)
bool SetIterator::valid() const 
{
	if (this->currentElement < 0 || this->currentElement >= this->set.size())
		return false;
	return true;
}

void SetIterator::kStepsPrev(int k)
{
	if (this->valid() && set.next[getCurrent()] < k)
		this->currentElement -= k;
	else
		throw std::exception("Invalid iterator");
}



