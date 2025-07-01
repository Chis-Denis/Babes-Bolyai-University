#pragma once

template <typename T>
class DynamicVector
{
private:
	T* elems;
	int size;
	int capacity;
	void resize();

public:
	// constructor
	DynamicVector(int initial_capacity = 10);

	// copy constructor
	DynamicVector(const DynamicVector& vector_to_copy);

	// destructor
	~DynamicVector();

	// overloading assignment operator
	DynamicVector& operator=(const DynamicVector& vector_to_copy);

	//methods
	void addElemToVector(const T& elem);
	void deleteElemFromVector(int index_to_remove);
	void updateElemFromVector(int index_to_update, T new_elem);
	int getSize() const;
	int findElemInVector(T elem);
	T getElemFromPosition(int position);
	void setElemAtPosition(int position, T elem);

};
/*
creates a dynamic vector with a given initial capacity
*/
template <typename T>
DynamicVector<T>::DynamicVector(int initial_capacity) : size{ 0 }, capacity{ initial_capacity }
{
	this->elems = new T[this->capacity];
}
/*
copy constructor

*/
template <typename T>
DynamicVector<T>::DynamicVector(const DynamicVector& vector_to_copy)
{
	this->size = vector_to_copy.size;
	this->capacity = vector_to_copy.capacity;
	this->elems = new T[this->capacity];
	for (int i = 0; i < this->size; i++)
		this->elems[i] = vector_to_copy.elems[i];
}
/*
destructor

*/
template <typename T>
DynamicVector<T>::~DynamicVector()
{
	delete[] this->elems;
}
/*
overloading assignment operator
returns a dynamic vector that is a copy of the one given as a parameter
*/
template <typename T>
DynamicVector<T>& DynamicVector<T>::operator=(const DynamicVector& vector_to_copy)
{


	this->size = vector_to_copy.size;
	this->capacity = vector_to_copy.capacity;

	delete[] this->elems;
	this->elems = new T[this->capacity];
	for (int i = 0; i < this->size; i++)
		this->elems[i] = vector_to_copy.elems[i];

	return *this;
}
/*
resizes the dynamic vector by doubling its capacity

*/
template <typename T>
void DynamicVector<T>::resize()
{
	int temp_capacity = this->capacity * 2;
	T* temp_elems = new T[temp_capacity];
	if (temp_elems == nullptr)
		return;
	this->capacity = temp_capacity;
	for (int i = 0; i < this->size; i++)
		temp_elems[i] = this->elems[i];
	delete[] this->elems;
	this->elems = temp_elems;

}
/*
adds an element to the dynamic vector

*/
template <typename T>
void DynamicVector<T>::addElemToVector(const T& elem)
{
	if (this->size == this->capacity)
		this->resize();
	this->elems[this->size] = elem;
	this->size++;
}

template <typename T>
void DynamicVector<T>::deleteElemFromVector(int index_to_remove)
{
	if (index_to_remove < 0 || index_to_remove >= this->size)
		return;
	T* temp_elems = new T[this->capacity];
	for (int i = 0; i < index_to_remove; i++)
		temp_elems[i] = this->elems[i];
	for (int i = index_to_remove + 1; i < this->size; i++)
		temp_elems[i - 1] = this->elems[i];
	delete[] this->elems;
	this->elems = temp_elems;
	this->size--;
}
/*
updates an element from the dynamic vector

*/
template <typename T>
void DynamicVector<T>::updateElemFromVector(int index_to_update, T new_elem)
{
	if (index_to_update < 0 || index_to_update >= this->size)
		return;
	this->elems[index_to_update] = new_elem;
}
/*
returns the size of the dynamic vector
*/
template <typename T>
int DynamicVector<T>::getSize() const
{
	return this->size;
}
/*
returns the position of an element in the dynamic vector

*/
template <typename T>
int DynamicVector<T>::findElemInVector(T elem)
{
	for (int i = 0; i < this->size; i++)
		if (this->elems[i] == elem)
			return i;
	return -1;
}
/*
returns the element from a given position
*/
template <typename T>
T DynamicVector<T>::getElemFromPosition(int position)
{
	return this->elems[position];
}

/*
sets an element at a given position in the vector
*/
template <typename T>
void DynamicVector<T>::setElemAtPosition(int position, T elem)
{
	if (position < 0 || position >= this->size)
		return;
	this->elems[position] = elem;
}
