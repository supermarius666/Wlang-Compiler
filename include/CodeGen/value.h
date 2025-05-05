#ifndef WLANG_VALUE_H
# define WLANG_VALUE_H

# include "common.h"

typedef double Value;

typedef struct {
	int 	capacity;
	int		count;
	Value	*values;
}	ValueArray;

/* methods */

/**
 * @brief Initializes a ValueArray structure.
 * 
 * This function sets up the initial state of a ValueArray, preparing it
 * for use. It ensures that the array is properly initialized and ready
 * to store values.
 * 
 * @param array A pointer to the ValueArray to be initialized.
 */
void	initValueArray(ValueArray *array);


/**
 * @brief Frees the memory allocated for a ValueArray.
 * 
 * This function releases all resources associated with the given ValueArray,
 * ensuring that any dynamically allocated memory within the array is properly
 * deallocated to prevent memory leaks.
 * 
 * @param array A pointer to the ValueArray to be freed.
 */
void	freeValueArray(ValueArray *array);


/**
 * @brief Writes a value into a ValueArray.
 * 
 * This function appends a given Value to the specified ValueArray.
 * It ensures that the ValueArray is updated to include the new Value.
 * 
 * @param array A pointer to the ValueArray where the Value will be written.
 * @param value A pointer to the Value to be added to the array.
 */
void	writeValueArray(ValueArray *array, Value value);

void	printValue(Value value);

#endif