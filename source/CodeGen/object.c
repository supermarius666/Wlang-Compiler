#include "../../include/CodeGen/object.h"
#include "../../include/CodeGen/value.h"
#include "../../include/CodeGen/memory.h"
#include "../../include/VM/vm.h"


#include <stdio.h>
#include <string.h>



Global globals[MAX_GLOBALS];
int globalCount = 0;

/* per evitare di fare il cast da void * a ObjString*/
#define ALLOCATE_OBJ(type, objectType) \
    ((type *)allocateObject(sizeof(type), objectType))

/* alloca l'getto sulla heap */
static Obj *allocateObject(size_t size, ObjType type)
{
	Obj	*object = (Obj *)reallocate(NULL, 0, size);
	object->type = type;
	
	object->next = vm.objects;
	vm.objects = object;
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

ObjString	*takeString(char *chars, int lenght)
{
	return (allocateString(chars, lenght));
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