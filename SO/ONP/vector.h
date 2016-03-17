/** @file
    @author Jakub Staroń, nr indeksu 361025
    @copyright Jakub Staroń
    @date 2015-05-23
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

struct Vector;

typedef struct Vector * Vector_pointer;
typedef const struct Vector * Vector_const_pointer;

/**
 * Typ wyliczeniowy określający tryb pamięci w jakim działa dynamiczna tablica.
 */
enum VectorMemoryMode
{
	VECTOR_GREEDY, // Capacity increase by one every time.
	VECTOR_DEFAULT // Capacity increase times constant.
};

/**
 * Returns new, empty vector.
 * size_t should be size of stored elements
 */
Vector_pointer Vector_new(size_t elementSize, enum VectorMemoryMode memoryMode);

/**
 * Destruction of vector.
 */
void Vector_free(Vector_pointer this);

/**
 * Increase capacity to be at least target_capacity big.
 */
void Vector_reserve(Vector_pointer this, size_t target_capacity);

/**
 * Adds new element at the end of the vector. Pointer should
 * point to insering element.
 */
void Vector_pushBack(Vector_pointer this, const void* pointer);

/**
 * Adds new elements at the end of the vector.
 */
void Vector_append(Vector_pointer this, const void* pointer, size_t count);

/**
 * Deletes one element from back of the vector.
 */
void Vector_popBack(Vector_pointer this);

/**
 * Adds new element at specified position.
 */
void Vector_insert(Vector_pointer this, const void* pointer, size_t index);

/**
 * Adds count new elements at specified position.
 */
void Vector_insertN(Vector_pointer this, const void* pointer, size_t index, size_t count);


/**
 * Removes one element from specified position.
 */
void Vector_remove(Vector_pointer this, size_t index);

/**
 * Removes count elements from specified position.
 */
void Vector_removeN(Vector_pointer this, size_t index, size_t count);

/**
 * Returns pointer to the element at specified position.
 */
void* Vector_at(Vector_const_pointer this, size_t index);

/**
 * Returns size of vector.
 */
size_t Vector_size(Vector_const_pointer this);

/**
 * Returns if vector is empty.
 */
bool Vector_empty(Vector_const_pointer this);

/**
 * Returns vector capacity.
 */
size_t Vector_capacity(Vector_const_pointer this);

/**
 * Cleans vector.
 */
void Vector_clear(Vector_pointer this);

/**
 * Returns pointer to begin of vector data.
 */
void* Vector_data(Vector_pointer this);

/**
 * Forces vector to shrink to match the size.
 */
void Vector_shrinkToFit(Vector_pointer this);

/**
 * Returns pointer to last element in vector.
 */
void* Vector_back(Vector_const_pointer this);

/**
 * Returns pointer to first element in vector.
 */
void* Vector_front(Vector_const_pointer this);

/**
 * Helping macros.
 */

#define VECTOR_AT(vector, index, type) (*((type*)(Vector_at((vector), (index)))))
#define VECTOR_BACK(vector, type) (*((type*)(Vector_back((vector)))))
#define VECTOR_FRONT(vector, type) (*((type*)(Vector_front((vector)))))

#endif /* __VECTOR__ */
