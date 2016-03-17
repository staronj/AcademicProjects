/** @file
    @author Jakub Staroń, nr indeksu 361025
    @copyright Jakub Staroń
    @date 2015-05-23
 */

#include <string.h>
#include <assert.h>
#include "vector.h"
#include "safe_memory_operations.h"

struct Vector
{
	size_t size;
	size_t capacity;
	size_t elementSize;
	enum VectorMemoryMode memoryMode;
	void* elements;
};

static size_t CalculateCapacity(size_t current_capacity, size_t desired_capacity, enum VectorMemoryMode memoryMode) {
	if (current_capacity >= desired_capacity) {
		return current_capacity;
	}
	else if (memoryMode == VECTOR_GREEDY) {
		return desired_capacity;
	}
	else {
		size_t result = 1;
		while (result < desired_capacity)
			result *= 2;
		return result;
	}
}

Vector_pointer Vector_new(size_t elementSize, enum VectorMemoryMode memoryMode) {
	Vector_pointer this = safe_raw_allocate(1, sizeof(struct Vector));
  this->size = 0;
  this->capacity = 0;
  this->elements = NULL;
  this->elementSize = elementSize;
  this->memoryMode = memoryMode;
  return this;
}

void Vector_free(Vector_pointer this) {
	if(this == NULL)
		return;
	free(this->elements);
	this->elements = NULL;
	this->size = 0;
	this->capacity = 0;
	free(this);
}

void Vector_reserve(Vector_pointer this, size_t target_capacity) {
	assert(this != NULL);
	size_t new_capacity = CalculateCapacity(this->capacity, target_capacity, this->memoryMode);
	if(this->capacity != new_capacity) {
		void* new_elements = safe_raw_allocate(new_capacity, this->elementSize);
		safe_raw_copy(new_elements, this->elements, this->size, this->elementSize);
		free(this->elements);
		this->elements = new_elements;
		this->capacity = new_capacity;
	}
}

void Vector_pushBack(Vector_pointer this, const void* pointer) {
	assert(this != NULL);
	Vector_reserve(this, this->size + 1);
	void* new_element_position = safe_raw_offset(this->elements, this->size, this->elementSize);
	safe_raw_copy(new_element_position, pointer, 1, this->elementSize);
	this->size++;
}

void Vector_append(Vector_pointer this, const void* pointer, size_t count) {
	Vector_insertN(this, pointer, Vector_size(this), count);
}

void Vector_popBack(Vector_pointer this) {
	assert(this != NULL);
	assert(this->size > 0);
	this->size--;
}

void Vector_insert(Vector_pointer this, const void* pointer, size_t index) {
	Vector_insertN(this, pointer, index, 1);
}

void Vector_insertN(Vector_pointer this, const void* pointer, size_t index, size_t count) {
	assert(this != NULL);
	assert(0 <= index && index <= this->size);
	Vector_reserve(this, this->size + count);

	void* new_elements_position = safe_raw_offset(this->elements, index, this->elementSize);
	safe_raw_move(new_elements_position, this->size - index, count, this->elementSize);
	safe_raw_copy(new_elements_position, pointer, count, this->elementSize);

	this->size += count;
}

void Vector_remove(Vector_pointer this, size_t index) {
	Vector_removeN(this, index, 1);
}

void Vector_removeN(Vector_pointer this, size_t index, size_t count) {
	assert(this != NULL);
	assert(0 <= index && index + count <= this->size);
	void* next_elements_position = safe_raw_offset(this->elements, index + count, this->elementSize);
	ptrdiff_t offset = -((ptrdiff_t)count);
	safe_raw_move(next_elements_position, this->size - (index + count), offset, this->elementSize);
	this->size -= count;
}

void* Vector_at(Vector_const_pointer this, size_t index) {
	assert(this != NULL);
	assert(0 <= index);
	assert(index < this->size);
	return safe_raw_offset(this->elements, index, this->elementSize);
}

size_t Vector_size(Vector_const_pointer this) {
	assert(this != NULL);
	return this->size;
}

bool Vector_empty(Vector_const_pointer this) {
  assert(this != NULL);
  return this->size == 0;
}

size_t Vector_capacity(Vector_const_pointer this) {
	assert(this != NULL);
	return this->capacity;
}

void Vector_clear(Vector_pointer this) {
	assert(this != NULL);
	this->size = 0;
}

void* Vector_data(Vector_pointer this) {
	assert(this != NULL);
	return this->elements;
}

void Vector_shrinkToFit(Vector_pointer this) {
	assert(this != NULL);
	if(this->size < this->capacity) {
		void* new_elements = safe_raw_allocate(this->size, this->elementSize);
		safe_raw_copy(new_elements, this->elements, this->size, this->elementSize);
		free(this->elements);
		this->elements = new_elements;
		this->capacity = this->size;
	}
}

void* Vector_back(Vector_const_pointer this) {
	assert(this != NULL);
	assert(this->size > 0);
	return Vector_at(this, this->size - 1);
}


void* Vector_front(Vector_const_pointer this) {
	assert(this != NULL);
	assert(this->size > 0);
	return Vector_at(this, 0);
}
