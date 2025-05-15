#include "../../include/CodeGen/value.h"
#include "../../include/CodeGen/memory.h"
#include "stdio.h"

void	initValueArray(ValueArray *array)
{
	array->capacity = 0;
	array->count = 0;
	array->values = NULL;
}

bool	valuesEqual(Value a, Value b)
{
	if (a.type != b.type) return (false);

	switch (a.type)
	{
	case VAL_BOOL: return (AS_BOOL(a) == AS_BOOL(b));
	case VAL_NIL: return (true);
	case VAL_NUMBER: return (AS_NUMBER(a) == AS_NUMBER(b));
			
	default:
		return (false);
	}
}

void	freeValueArray(ValueArray *array)
{
	FREE_ARRAY(Value, array->values, array->capacity);
	initValueArray(array);
}

void	writeValueArray(ValueArray *array, Value value)
{
	int oldCapacity;

	if (array->capacity < array->count + 1)
	{
		oldCapacity = array->capacity;
		array->capacity = GROW_CAPACITY(oldCapacity);
		array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
	}
	array->values[array->count] = value;
	array->count++;
}

void	printValue(Value value)
{
	switch (value.type)
	{
		case VAL_BOOL:
		{
			printf(AS_BOOL(value) ? "vero" : "falso");
			break;
		}
		case VAL_NIL: printf("nulla"); break; 
		case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;  /* uso %g perché sceglie in automatico se servono le cifre decimali o meno */
		default:
			break;
	}
}