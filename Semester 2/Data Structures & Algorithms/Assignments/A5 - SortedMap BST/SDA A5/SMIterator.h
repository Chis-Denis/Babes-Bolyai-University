#pragma once
#include "SortedMap.h"

//DO NOT CHANGE THIS PART
class SMIterator{
	friend class SortedMap;
private:
	const SortedMap& map;
	SMIterator(const SortedMap& mapionar);

    // Stack to hold nodes for in-order traversal
    struct StackNode {
        SortedMap::Node* node;
        StackNode* next;
        StackNode(SortedMap::Node* node, StackNode* next) : node(node), next(next) {}
    };
    StackNode* stack;

    // Push a node onto the stack
    void push(SortedMap::Node* node);

    // Pop a node from the stack
    SortedMap::Node* pop();

    // Move to the next node in in-order traversal
    void moveNext();

public:
	void first();
	void next();
	bool valid() const;
    TElem getCurrent() const;
};

