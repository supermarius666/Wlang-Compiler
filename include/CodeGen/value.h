#ifndef WLANG_VALUE_H
# define WLANG_VALUE_H

# include "common.h"

/* forward declaration --> dichiaro la struct Obj, come se fosse una classe */
typedef struct Obj	Obj;
/* forward declaration della struct che rappresenta l'oggetto di tipo stringa --> deriva da Obj */
typedef struct ObjString ObjString;

/* tipi di valori che posso avere nel linguaggio */
typedef enum 
{
	VAL_BOOL,		/* booleani --> vero, false */
	VAL_NIL,		/* nulla */
	VAL_NUMBER,  	/* numeri */
	VAL_OBJ,		/* questo è un tipo di valore particolare che server per rappresentare 
					   cose cose allocate in heap, cose grandi che possono variare, come le stringhe, funzioni, ...*/ 
}	ValueType;

/* union per salvare in memoria il valore */
typedef struct 
{	
	ValueType	type;		/* tipo del valore */
	union					/* union per salvare il valore */ 	
	{	
		bool	boolean;
		double	number;
		Obj 	*obj;
	}	as;
}	Value;

/* per verificare se l'oggetto value è di un tipo particolare */
# define IS_BOOL(value)		((value).type == VAL_BOOL)
# define IS_NIL(value)		((value).type == VAL_NIL)
# define IS_NUMBER(value)	((value).type == VAL_NUMBER)
# define IS_OBJ(value)		((value).type == VAL_OBJ)

/* macro per prendere il valore da un oggetto Value
*	
*	// equivalente 	
*	bool b = val.as.boolean;
*/
# define AS_BOOL(value)		((value).as.boolean)
# define AS_NUMBER(value)	((value).as.number)
# define AS_OBJ(value)  	((value).as.obj)


/*  designated initializer --> macro per creare un oggetto di tipo value
*	setta il campo type a VAL_
*	imposta il valore a value
*
*	equivalente a:
*
*	Value v;
*	v.type = VAL_NUMBER;
*	v.as.number = 3.14;
*
*	Value v = {
*    .type = VAL_NUMBER,
*    .as = {
*        .number = 3.14
*    }
* };
*
*/
# define BOOL_VAL(value)	((Value) {VAL_BOOL, {.boolean = value}})
# define NIL_VAL			((Value) {VAL_NIL, {.number = 0}})
# define NUMBER_VAL(value)	((Value) {VAL_NUMBER, {.number = value}})
# define OBJ_VAL(object)	((Value) {VAL_OBJ, {.obj = (Obj *)object}})

typedef struct 
{
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

/* gestisce == tra i vari tipi di valori */
bool	valuesEqual(Value a, Value b);

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

# include "object.h"
#endif