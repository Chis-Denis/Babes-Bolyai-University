#include "ListIterator.h"
#include "IndexedList.h"
#include <exception>
#include <iostream>

//best case: O(1), worst case: O(1), total case: O(1)
ListIterator::ListIterator(const IndexedList& list) : list(list){

    this->currentNode = list.head;

}

//best case: O(1), worst case: O(1), total case: O(1)
void ListIterator::first(){

	this->currentNode = list.head;

}

//best case: O(1), worst case: O(1), total case: O(1)
void ListIterator::previous() {
	if (this->currentNode == nullptr) {
		throw std::exception();
	}

	this->currentNode = this->currentNode->prev;
}

//best case: O(1), worst case: O(1), total case: O(1)
void ListIterator::next(){

	if (this->currentNode == nullptr) {
		throw std::exception();
	}

	//std::cout << currentNode->data << std::endl;

	this->currentNode = this->currentNode->next;
}

//best case: O(1), worst case: O(1), total case: O(1)
bool ListIterator::valid() const{

	return this->currentNode != nullptr;

}

//best case: O(1), worst case: O(1), total case: O(1)
TElem ListIterator::getCurrent() const{

	if (this->currentNode == nullptr) {
		throw std::exception();
	}

	return this->currentNode->data;

}