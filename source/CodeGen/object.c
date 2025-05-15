#include "../../include/CodeGen/object.h"
#include "../../include/CodeGen/value.h"
#include "../../include/CodeGen/memory.h"
#include "../../include/VM/vm.h"


#include <stdio.h>
#include <string.h>


/* per evitare di fare il cast da void * a ObjString*/
#define ALLOCATE_OBJ(type, objectType) \
    ((type *)allocateObject(sizeof(type), objectType))

/* alloca l'getto sulla heap */
static Obj *allocateObject(size_t size, ObjType type)
{
	Obj	*object = (Obj *)reallocate(NULL, 0, size);
	object->type = type;
	
	return (object);
}

/* crea una nuova stringa e la alloca in heap; vengono inizializzati i suoi campi */
static ObjString *allocateString(char *chars, int lenght)
{
	ObjString	*string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
	string->lenght = lenght;
	string->chars = chars;

	return (string);
}

ObjString	*copyString(const char *chars, int lenght)
{
	char	*heapChars = ALLOCATE(char, lenght + 1);
	memcpy(heapChars, chars, lenght);
	heapChars[lenght] = '\0';

	return (allocateString(heapChars, lenght));
}

void printObject(Value value)
{
	switch (OBJ_TYPE(value))
	{
		/* per stampare la stringa */
		case OBJ_STRING:
			printf("%s", AS_CSTRING(value));
			break;
		
		default:
			break;
	}
}