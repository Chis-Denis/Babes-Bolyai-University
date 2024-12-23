#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>
using namespace std;

// Best case: Theta(1), Worst case: Theta(1), Total case: Theta(1)
SortedMap::SortedMap(Relation r) : root(nullptr), size_(0), relation(r) {}

// Best case: Theta(1), Worst case: Theta(n), Total case: O(n)
void SortedMap::destroyRecursive(Node* node) {
	if (node != nullptr) {
		destroyRecursive(node->left);
		destroyRecursive(node->right);
		delete node;
	}
}

// Best case: Theta(1), Worst case: Theta(n), Total case: O(n)
SortedMap::Node* SortedMap::findMin(Node* node)
{
	while (node->left != nullptr) {
		node = node->left;
	}
	return node;
}

// Best case: Theta(1), Worst case: Theta(n), Total case: O(n)
TValue SortedMap::add(TKey k, TValue v) 
{
	Node** current = &root;
	while (*current != nullptr) 
	{
		if (k == (*current)->elem.first) 
		{
			// Key already exists in the map
			TValue oldValue = (*current)->elem.second;
			(*current)->elem.second = v; // Update the value associated with the key
			return oldValue;
		}
		else if (relation(k, (*current)->elem.first)) {
			current = &(*current)->left;
		}
		else {
			current = &(*current)->right;
		}
	}
	// Key does not exist in the map
	*current = new Node(k, v);
	size_++;
	return NULL_TVALUE;
}

// Best case: Theta(1), Worst case: Theta(n), Total case: O(n)
TValue SortedMap::search(TKey k) const {
	Node* current = root;
	while (current != nullptr) {
		if (k == current->elem.first) {
			return current->elem.second;
		}
		if (relation(k, current->elem.first)) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}
	return NULL_TVALUE;
}

// Best case: Theta(1), Worst case: Theta(n), Total case: O(n)
TValue SortedMap::remove(TKey k) 
{
	Node** current = &root;

	while (*current != nullptr) 
	{
		if (k == (*current)->elem.first) 
		{
			// Key found, remove it
			Node* nodeToRemove = *current;
			TValue oldValue = nodeToRemove->elem.second;

			if (nodeToRemove->left == nullptr) {
				*current = nodeToRemove->right;
			}
			else if (nodeToRemove->right == nullptr) {
				*current = nodeToRemove->left;
			}
			else 
			{
				Node* minNode = findMin(nodeToRemove->right);
				nodeToRemove->elem = minNode->elem;
				Node** minParent = &nodeToRemove->right;

				while ((*minParent)->left != nullptr) 
				{
					minParent = &(*minParent)->left;
				}

				*minParent = minNode->right;
				nodeToRemove = minNode;
			}

			delete nodeToRemove;
			size_--;
			return oldValue; // Return the old value associated with the removed key

		}
		else if (relation(k, (*current)->elem.first)) {
			current = &(*current)->left;
		}
		else {
			current = &(*current)->right;
		}
	}
	// Key not found in the map
	return NULL_TVALUE;
}

// Best case: Theta(1), Worst case: Theta(1), Total case: Theta(1)
int SortedMap::size() const {
	return size_;
}

// Best case: Theta(1), Worst case: Theta(1), Total case: Theta(1)
bool SortedMap::isEmpty() const {
	return size_ == 0;
}

// Best case: Theta(1), Worst case: Theta(1), Total case: Theta(1)
SMIterator SortedMap::iterator() const {
	return SMIterator(*this);
}

// Best case: Theta(1), Worst case: Theta(n), Total case: O(n)
int SortedMap::getKeyRange() const {
	if (root == nullptr) {
		return -1;
	}
	Node* current = root;
	while (current->left != nullptr) {
		current = current->left;
	}
	TKey minKey = current->elem.first;
	current = root;
	while (current->right != nullptr) {
		current = current->right;
	}
	TKey maxKey = current->elem.first;
	return maxKey - minKey;
}

// Best case: Theta(1), Worst case: Theta(n), Total case: O(n)
SortedMap::~SortedMap() {
	destroyRecursive(root);
}
