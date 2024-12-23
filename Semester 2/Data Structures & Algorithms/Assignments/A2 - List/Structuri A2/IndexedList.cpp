#include <exception>

#include "IndexedList.h"
#include "ListIterator.h"

//best case: O(1), worst case: O(1), total case: O(1)
IndexedList::IndexedList() {

    this->head = nullptr;
    this->tail = nullptr;
    this->length = 0;

}

//best case: O(1), worst case: O(1), total case: O(1)
int IndexedList::size() const {

    return this->length;
}


//best case: O(1), worst case: O(1), total case: O(1)
bool IndexedList::isEmpty() const {

    return this->length == 0;

}

//best case: O(n), worst case: O(n), total case: O(n)
TElem IndexedList::getElement(int pos) const {

    if (pos < 0 || pos >= this->length) {
		throw std::exception();
	}

	Node* currentNode = this->head;

    for (int i = 0; i < pos; i++) {
		currentNode = currentNode->next;
	}

	return currentNode->data;

}

//best case: O(n), worst case: O(n), total case: O(n)
TElem IndexedList::setElement(int pos, TElem e) {

    if (pos < 0 || pos >= this->length) {
        throw std::exception();
    }

    Node* currentNode = this->head;

    for (int i = 0; i < pos; i++) {
		currentNode = currentNode->next;
	}

    TElem oldValue = currentNode->data;
    currentNode->data = e;

    return oldValue;

}

//best case: O(1), worst case: O(1), total case: O(1)
void IndexedList::addToEnd(TElem e) {

    Node* newNode = new Node;
	newNode->data = e;
	newNode->next = nullptr;
	newNode->prev = this->tail;

    if (this->length == 0) {
		this->head = newNode;
    }
    else {
		this->tail->next = newNode;
	}

	this->tail = newNode;
	this->length++;

}

//best case: O(n), worst case: O(n), total case: O(n)
void IndexedList::addToPosition(int pos, TElem e) {

    if (pos < 0 || pos > this->length) {
		throw std::exception();
	}

	Node* newNode = new Node;
	newNode->data = e;

	if (pos == 0) {
		newNode->next = this->head;
		newNode->prev = nullptr;
		this->head = newNode;
	}
	else {
		Node* currentNode = this->head;

		for (int i = 0; i < pos - 1; i++) {
			currentNode = currentNode->next;
		}

		newNode->next = currentNode->next;
		newNode->prev = currentNode;
		currentNode->next = newNode;

		if (newNode->next == nullptr) {
			this->tail = newNode;
		}
	}

	this->length++;

}

//best case: O(1), worst case: O(pos), total case: theta(pos)
TElem IndexedList::remove(int pos) {
	if (pos < 0 || pos >= this->length) {
		throw std::exception();
	}

	Node* currentNode = this->head;
	Node* previousNode = nullptr;

	if (pos == 0) {
		this->head = this->head->next;
	}
	else {
		for (int i = 0; i < pos; i++) {
			previousNode = currentNode;
			currentNode = currentNode->next;
		}

		if (currentNode == this->tail) {
			this->tail = previousNode;
		}

		previousNode->next = currentNode->next;
	}

	TElem removedElement = currentNode->data;
	delete currentNode;
	this->length--;

	return removedElement;
}


//best case: O(n), worst case: O(n), total case: O(n)
int IndexedList::search(TElem e) const{

	Node* currentNode = this->head;

	for (int i = 0; i < this->length; i++) {
		if (currentNode->data == e) {
			return i;
		}

		currentNode = currentNode->next;
	}

	return -1;

}

//best case: O(1), worst case: O(1), total case: O(1)
ListIterator IndexedList::iterator() const {
    return ListIterator(*this);        
}

//best case: O(n), worst case: O(n), total case: O(n)
IndexedList::~IndexedList() {

	Node* currentNode = this->head;

	while (currentNode != nullptr) {
		Node* nextNode = currentNode->next;
		delete currentNode;
		currentNode = nextNode;
	}
}

void IndexedList::removeBetween(int start_position, int end_position) {

	if (start_position < 0 || start_position >= this->length || end_position < 0 || end_position >= this->length || start_position > end_position) {
		throw std::exception();
	}

	Node* currentNode = this->head;

	for (int i = 0; i < start_position; i++) {
		currentNode = currentNode->next;
	}

	Node* startNode = currentNode;

	for (int i = start_position; i < end_position; i++) {
		currentNode = currentNode->next;
	}

	Node* endNode = currentNode;

	if (start_position == 0) {
		this->head = endNode->next;
	}
	else {
		currentNode = startNode->prev;
		currentNode->next = endNode->next;
	}

	if (end_position == this->length - 1) {
		this->tail = startNode->prev;
	}
	else {
		currentNode = endNode->next;
		currentNode->prev = startNode->prev;
	}

	currentNode = startNode;

	while (currentNode != endNode) {
		Node* nextNode = currentNode->next;
		delete currentNode;
		currentNode = nextNode;
		this->length--;
	}

	delete endNode;
	this->length--;

}