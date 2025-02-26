#include "Set.h"
#include "SetITerator.h"
#include <iostream>

// Complexity: theta(1)
Set::Set() 
{
	this->capacity = 0;
	this->head = -1;
	this->firstEmpty = 0;
	this->elements = new TElem[capacity];
	this->next = new int[capacity];
	this->prev = new int[capacity];
}

// Complexity: best case: theta(1), worst case: theta(n), total complexity: O(n)
bool Set::add(TElem elem) 
{
	if (this->search(elem))
		return false;

	if (this->capacity == 0)
	{
		this->elements = new TElem[1];
		this->next = new int[1];
		this->elements[0] = elem;
		this->next[0] = -1;
		this->capacity++;

		this->head = 0;
		this->firstEmpty = -1;
		return true;
	}

	if (this->firstEmpty == -1)
	{
		TElem* newElements = new TElem[this->capacity * 2];
		int* newNext = new int[this->capacity * 2];
		for (int i = 0; i < this->capacity; i++)
		{
			newElements[i] = this->elements[i];
			newNext[i] = this->next[i];
		}
		for (int i = this->capacity; i < this->capacity * 2; i++)
		{
			newNext[i] = i + 1;
		}
		newNext[this->capacity * 2 - 1] = -1;
		delete[] this->elements;
		delete[] this->next;
		this->elements = newElements;
		this->next = newNext;
		this->firstEmpty = this->capacity;
		this->capacity *= 2;
	}

	int emptyIndex = this->firstEmpty;
	this->firstEmpty = this->next[this->firstEmpty];

	// Add the element to the list
	this->elements[emptyIndex] = elem;
	this->next[emptyIndex] = -1;

	if (this->head == -1)
	{
		this->head = emptyIndex;
		return true;
	}

	int current = this->head;
	while (this->next[current] != -1) 
	{
		current = this->next[current];
	}
	this->next[current] = emptyIndex;

	/*for (int i = 0; i <= size() - 1; i++)
		std::cout << this->elements[i] << " ";
	std::cout << std::endl;

	for (int i = 0; i <= size() - 1; i++)
		std::cout << this->next[i] << " ";
	std::cout << std::endl;

	std::cout << std::endl;*/

	return true;
}

// Complexity: best case: theta(1), worst case: theta(n), total complexity: O(n)
bool Set::remove(TElem elem) 
{
	if (this->capacity == 0)
		return false;

	int current = this->head;
	int prev = -1;

	if (elements[current] == elem)
	{
		this->head = this->next[current];
		this->elements[current] = NULL;
		this->next[current] = -1;
		return true;
	}

	while (current != -1) 
	{
		if (elements[current] == elem) 
		{
			while (this->next[current] != -1)
			{
				this->elements[current] = this->elements[this->next[current]];
				current = this->next[current];
			}
			//remove the last elemnt from elements and next
			this->elements[size()-1] = NULL;
			this->next[size()-2] = -1;

			return true;
		}
		else
		{
			prev = current;
			current = this->next[current];
		}
	}

	/*for (int i = 0; i <= size() - 1; i++)
		std::cout << this->elements[i] << " ";
	std::cout << std::endl;

	for (int i = 0; i <= size() - 1; i++)
		std::cout << this->next[i] << " ";
	std::cout << std::endl;

	std::cout << std::endl;*/

	return false;  // Element not found in the list
}

// Complexity: best case: theta(1), worst case: theta(n), total complexity: O(n)
bool Set::search(TElem elem) const 
{
	if (this->capacity == 0)
		return false;

	int current = this->head;
	while (current != -1) {
		if (elements[current] == elem)
			return true;  // Element found in the list
		current = this->next[current];
	}

	return false;  // Element not found in the list
}


// Complexity: best case: theta(1), worst case: theta(n), total complexity: O(n)
int Set::size() const 
{
	if (this->next[0] == -1)
		return 0;
	for (int i = 0; i < this->capacity; i++)
	{
		if (this->next[i] == -1)
			return i + 1;
	}
}

// Complexity: theta(1)
bool Set::isEmpty() const 
{
	return this->size() == 0;
}

// Complexity: theta(1)
Set::~Set() 
{
	delete[] this->elements;
	delete[] this->next;
}

// Complexity: theta(1)
SetIterator Set::iterator() const {
	return SetIterator(*this);
}


