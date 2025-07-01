#pragma once
#include <stdexcept> // Added for std::out_of_range

template <typename T>
class DynamicVector {
private:
    T* elements;
    int size;
    int capacity;

    // Private method to resize the array when needed
    void resize();

public:
    // Default constructor
    DynamicVector(int capacity = 10);

    // Copy constructor
    DynamicVector(const DynamicVector& other);

    // Destructor
    ~DynamicVector();

    // Assignment operator
    DynamicVector& operator=(const DynamicVector& other);

    // Add element at the end
    void add(const T& element);

    // Remove element at specified index
    void remove(int index);

    // Get element at index
    T& get(int index);
    const T& get(int index) const;

    // Get size
    int getSize() const;

    // Get capacity
    int getCapacity() const;

    // Clear the vector
    void clear();

    // Check if vector is empty
    bool isEmpty() const;

    // Operator [] overload for direct access
    T& operator[](int index);
    const T& operator[](int index) const;
};

// Implementation of template class must be in header file
template <typename T>
DynamicVector<T>::DynamicVector(int capacity) {
    this->size = 0;
    this->capacity = capacity;
    this->elements = new T[capacity];
}

template <typename T>
DynamicVector<T>::DynamicVector(const DynamicVector& other) {
    this->size = other.size;
    this->capacity = other.capacity;
    this->elements = new T[other.capacity];
    for (int i = 0; i < this->size; i++)
        this->elements[i] = other.elements[i];
}

template <typename T>
DynamicVector<T>::~DynamicVector() {
    delete[] this->elements;
}

template <typename T>
DynamicVector<T>& DynamicVector<T>::operator=(const DynamicVector& other) {
    if (this != &other) {
        delete[] this->elements;
        this->size = other.size;
        this->capacity = other.capacity;
        this->elements = new T[other.capacity];
        for (int i = 0; i < this->size; i++)
            this->elements[i] = other.elements[i];
    }
    return *this;
}

template <typename T>
void DynamicVector<T>::resize() {
    T* newElements = new T[this->capacity * 2];
    for (int i = 0; i < this->size; i++)
        newElements[i] = this->elements[i];
    
    delete[] this->elements;
    this->elements = newElements;
    this->capacity *= 2;
}

template <typename T>
void DynamicVector<T>::add(const T& element) {
    if (this->size == this->capacity)
        this->resize();
    this->elements[this->size++] = element;
}

template <typename T>
void DynamicVector<T>::remove(int index) {
    if (index < 0 || index >= this->size)
        throw std::out_of_range("Index out of range");
    
    for (int i = index; i < this->size - 1; i++)
        this->elements[i] = this->elements[i + 1];
    this->size--;
}

template <typename T>
T& DynamicVector<T>::get(int index) {
    if (index < 0 || index >= this->size)
        throw std::out_of_range("Index out of range");
    return this->elements[index];
}

template <typename T>
const T& DynamicVector<T>::get(int index) const {
    if (index < 0 || index >= this->size)
        throw std::out_of_range("Index out of range");
    return this->elements[index];
}

template <typename T>
int DynamicVector<T>::getSize() const {
    return this->size;
}

template <typename T>
int DynamicVector<T>::getCapacity() const {
    return this->capacity;
}

template <typename T>
void DynamicVector<T>::clear() {
    this->size = 0;
}

template <typename T>
bool DynamicVector<T>::isEmpty() const {
    return this->size == 0;
}

template <typename T>
T& DynamicVector<T>::operator[](int index) {
    if (index < 0 || index >= this->size)
        throw std::out_of_range("Index out of range");
    return this->elements[index];
}

template <typename T>
const T& DynamicVector<T>::operator[](int index) const {
    if (index < 0 || index >= this->size)
        throw std::out_of_range("Index out of range");
    return this->elements[index];
}