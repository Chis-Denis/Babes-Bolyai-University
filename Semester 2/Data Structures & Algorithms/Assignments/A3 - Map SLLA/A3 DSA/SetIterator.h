#pragma once
#include "Set.h"

class SetIterator
{
	//DO NOT CHANGE THIS PART
	friend class Set;
private:
	//DO NOT CHANGE THIS PART
	const Set& set;
	SetIterator(const Set& s);

	int currentElement;

public:
	void first();
	void next();
	void prev();
	void kStepsPrev(int k);
	TElem getCurrent();
	bool valid() const;
};


