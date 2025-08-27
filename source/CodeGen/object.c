#include "../../include/CodeGen/object.h"
#include "../../include/CodeGen/value.h"
#include "../../include/CodeGen/memory.h"
#include "../../include/CodeGen/table.h"
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
	
	object->next = vm.objects;
	vm.objects = object;
	return (object);
}

ObjFunction	*newFunction()
{
	ObjFunction *function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);

	function->arity = 0;
	function->name = NULL;
	initChunk(&function->chunk);
	return (function);
}


/* crea una nuova stringa e la alloca in heap; vengono inizializzati i suoi campi */
static ObjString *allocateString(char *chars, int lenght, uint32_t hash)
{
	ObjString	*string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
	string->lenght = lenght;
	string->chars = chars;
	string->hash = hash;

	tableSet(&vm.strings, string, NIL_VAL);

	return (string);
}

/* FNV-1a alogithm */
static uint32_t	hashString(const char *key, int lenght)
{
	uint32_t hash = 2166136261u;

	for (int i = 0; i < lenght; i++)
	{
		hash ^= key[i];
		hash *= 16777619;
	}

	return (hash);
}


ObjString	*takeString(char *chars, int lenght)
{
	uint32_t hash = hashString(chars, lenght);
	ObjString *interned = tableFindString(&vm.strings, chars, lenght, hash);

	if (interned != NULL)
	{
		FREE_ARRAY(char, chars, lenght + 1);
		return (interned);
	}

	return (allocateString(chars, lenght, hash));
}


ObjString	*copyString(const char *chars, int lenght)
{
	uint32_t hash = hashString(chars, lenght);
	ObjString *interned = tableFindString(&vm.strings, chars, lenght, hash);

	if (interned != NULL) return (interned);
	
	char *heapChars = ALLOCATE(char, lenght + 1);
	memcpy(heapChars, chars, lenght);
	heapChars[lenght] = '\0';

	return (allocateString(heapChars, lenght, hash));
}

/* funzione per stampare le funzioni */
static void printFunction(ObjFunction *function)
{
	if (function->name == NULL)
	{
		printf("<main>");
		return;
	}
	printf("<fn %s>", function->name->chars);
}

void printObject(Value value)
{
	switch (OBJ_TYPE(value))
	{
		/* per stampare la stringa */
		case OBJ_STRING:
		{
			printf("%s", AS_CSTRING(value));
			break;
		}
		
		case OBJ_FUNCTION:
		{
			printFunction(AS_FUNCTION(value));
			break;
		}

		default:
			break;
	}
}