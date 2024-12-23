#include "Map.h"
#include <iostream>

Map::Map() {
    this->capacity = 10;
    this->elements = new TElem[10];
    this->nrElements = 0;
    this->firstEmpty = 0;
    for (int i = 0; i < this->capacity; i++) {
        this->elements[i].first = -111111;
        this->elements[i].second = -111111;
    }
    this->next = new int[10];
    for (int i = 0; i < this->capacity; i++) {
        this->next[i] = NULL;
    }
}

int Map::hashFunction(TKey c) const {
    return c % this->capacity;
}

Map::~Map() {
    delete[] this->elements;
    delete[] this->next;
}

void Map::resizeHashTable() {
    int oldCapacity = this->capacity;
    this->capacity *= 2;
    TElem* oldElements = this->elements;
    int* oldNext = this->next;
    this->elements = new TElem[this->capacity];
    this->next = new int[this->capacity];

    // Reinitialize the new hash table
    for (int i = 0; i < this->capacity; i++) {
        this->elements[i].first = -111111;
        this->elements[i].second = -111111;
        this->next[i] = NULL;
    }

    // Rehash the elements
    for (int i = 0; i < oldCapacity; i++) {
        if (oldElements[i].first != -111111) {
            int newIndex = hashFunction(oldElements[i].first);
            if (this->elements[newIndex].first == -111111) {
                this->elements[newIndex] = oldElements[i];
            } else {
                int j = newIndex;
                while (this->next[j] != NULL) {
                    j = this->next[j];
                }
                this->next[j] = newIndex;
                this->elements[newIndex] = oldElements[i];
            }
        }
    }

    delete[] oldElements;
    delete[] oldNext;
}

TValue Map::add(TKey c, TValue v) {
    int index = hashFunction(c);
    if (this->elements[index].first == -111111) {
        this->elements[index] = TElem(c, v);
    } else {
        // Find the first empty place from the bottom
        int emptyIndex = find_empty_place_from_bottom(index);
        this->elements[emptyIndex] = TElem(c, v);
        // Update the next field of the previous pointing node of the chain
        int prev = index;
        while (this->next[prev] != NULL) {
            prev = this->next[prev];
        }
        this->next[prev] = emptyIndex;
    }
    this->nrElements++;

    // Resize hash table if load factor exceeds 0.75
    if ((double)this->nrElements / this->capacity > 0.75) {
        resizeHashTable();
    }

    return v;
}

TValue Map::search(TKey c) const {
    int index = hashFunction(c);
    int current = index;
    while (current != NULL) {
        if (this->elements[current].first == c) {
            return this->elements[current].second;
        }
        current = this->next[current];
    }
    return NULL_TVALUE;
}

TValue Map::remove(TKey c) {
    int index = hashFunction(c);
    int current = index;
    int prev = index;
    while (current != NULL) {
        if (this->elements[current].first == c) {
            // Found the element to delete
            TValue value = this->elements[current].second;
            this->elements[current].first = -111111;
            this->elements[current].second = -111111;

            // Update the next field of the previous pointing node of the chain
            if (current == index) {
                this->next[prev] = NULL;
            } else {
                this->next[prev] = this->next[current];
            }

            this->nrElements--;
            return value;
        }
        prev = current;
        current = this->next[current];
    }
    return NULL_TVALUE;
}

int Map::size() const {
    return this->nrElements;
}

bool Map::isEmpty() const {
    return this->nrElements == 0;
}
