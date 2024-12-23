#include "ExtendedTest.h"
#include "ShortTest.h"

#include "Map.h"


#include <iostream>
using namespace std;


int main() {
	testAll();
//	testAllExtended();

	cout << "That's all!" << endl;
	system("pause");
	return 0;
}

//23. ADT MAP - using  a hash table with coalesced chaining
/*Coalesced hashing is a collision avoidance technique when there is a fixed sized data. It is a combination of both Separate chaining and Open addressing. It uses the concept of Open Addressing(linear probing) to find first empty place for colliding element from the bottom of the hash table and the concept of Separate Chaining to link the colliding elements to each other through pointers. 

The hash function used is h=(key)%(total number of keys). Inside the hash table, each node has three fields:

h(key): The value of hash function for a key.
Data: The key itself.
Next: The link to the next colliding elements.
The basic operations of Coalesced hashing are:

INSERT(key): The insert Operation inserts the key according to the hash value of that key if that hash value in the table is empty otherwise the key is inserted in first empty place from the bottom of the hash table and the address of this empty place is mapped in NEXT field of the previous pointing node of the chain.(Explained in example below).
DELETE(Key): The key if present is deleted. Also if the node to be deleted contains the address of another node in hash table then this address is mapped in the NEXT field of the node pointing to the node which is to be deleted
SEARCH(key): Returns True if key is present, otherwise return False*/
