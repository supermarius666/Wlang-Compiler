#ifndef WLANG_MEMORY_H
# define WLANG_MEMORY_H

# include "common.h"

/* questa macro alloca memoria */
# define ALLOCATE(type, count)	\
	(type *)reallocate(NULL, 0, sizeof(type) * (count))

/**
 * 	GROW_CAPACITY(capacity):
 * 	 Calculates the new capacity for a dynamically
 *   allocated array or buffer. If the current capacity is less than 8, it
 *   returns 8. Otherwise, it doubles the current capacity.
 */
# define GROW_CAPACITY(capacity) \
	((capacity) < 8 ? 8 : (capacity) * 2)

/*
* 	GROW_ARRAY(type, pointer, oldCount, newCount): 
*	 Reallocates memory for
*    an array of a given type, adjusting its size from oldCount to newCount.
*    This macro uses the `reallocate` function to perform the memory operation.
*/
# define GROW_ARRAY(type, pointer, oldCount, newCount) \
	(type *) reallocate (pointer, sizeof(type) * (oldCount), \
		sizeof(type)  * (newCount))


/**
 * @brief Frees the memory allocated for an array of a specific type.
 *
 * This macro is used to deallocate memory for an array of a given type
 * and pointer. 
 * @param type The data type of the elements in the array.
 * @param pointer A pointer to the array to be freed.
 * @param oldCount The number of elements in the array that was allocated.
 */
# define FREE_ARRAY(type, pointer, oldCount) \
	reallocate(pointer, sizeof(type) * oldCount, 0)

/**
 * @brief Reallocates a block of memory to a new size.
 * 
 * This function adjusts the size of the memory block pointed to by `pointer`
 * to the new size specified by `newSize`. If the new size is larger than the
 * old size, the additional memory is uninitialized. If the new size is smaller,
 * the memory is truncated. If `pointer` is `nullptr`, this function behaves
 * like `malloc` and allocates a new block of memory of size `newSize`.
 * 
 * @param pointer A pointer to the memory block to be reallocated. Can be `nullptr`.
 * @param oldSize The current size of the memory block in bytes.
 * @param newSize The desired size of the memory block in bytes.
 * @return A pointer to the newly allocated memory block, or `nullptr` if the
 *         allocation fails.
 */
void	*reallocate(void *pointer, size_t oldSize, size_t newSize);


#endif