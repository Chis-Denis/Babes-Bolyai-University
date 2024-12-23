#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>

using namespace std;

// Best case: Theta(1), Worst case: Theta(1), Total case: Theta(1)
SMIterator::SMIterator(const SortedMap& m) : map(m), stack(nullptr) {
	first();
}

// Best case: Theta(1), Worst case: Theta(1), Total case: Theta(1)
void SMIterator::push(SortedMap::Node* node) {
	stack = new StackNode(node, stack);
}

// Best case: Theta(1), Worst case: Theta(1), Total case: Theta(1)
SortedMap::Node* SMIterator::pop() {
	if (stack == nullptr) {
		return nullptr;
	}
	StackNode* topNode = stack;
	SortedMap::Node* node = topNode->node;
	stack = topNode->next;
	delete topNode;
	return node;
}

// Best case: Theta(1), Worst case: Theta(n), Total case: O(n)
void SMIterator::moveNext() {
	if (stack == nullptr) {
		return;
	}
	SortedMap::Node* node = pop();
	if (node->right != nullptr) {
		node = node->right;
		while (node != nullptr) {
			push(node);
			node = node->left;
		}
	}
}

// Best case: Theta(n), Worst case: Theta(n), Total case: O(n)
void SMIterator::first(){
	while (stack != nullptr) {
		pop();
	}
	SortedMap::Node* node = map.root;
	while (node != nullptr) {
		push(node);
		node = node->left;
	}
}

// Best case: Theta(1), Worst case: Theta(n), Total case: O(n)
void SMIterator::next(){
	if (!valid()) {
		throw exception();
	}
	moveNext();
}

// Best case: Theta(1), Worst case: Theta(1), Total case: Theta(1)
bool SMIterator::valid() const{
	return stack != nullptr;
}

// Best case: Theta(1), Worst case: Theta(1), Total case: Theta(1)
TElem SMIterator::getCurrent() const{
	if (!valid()) {
		throw exception();
	}
	return stack->node->elem;
}


