#pragma once
#include "IndexedList.h"


class ListIterator{
    //DO NOT CHANGE THIS PART
	friend class IndexedList;
private:
	const IndexedList& list;
    Node* currentNode;

    ListIterator(const IndexedList& lista);
public:
    void first();
    void next();
    void previous();
    bool valid() const;
    TElem getCurrent() const;
};

