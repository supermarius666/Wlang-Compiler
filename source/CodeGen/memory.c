#include "../../include/CodeGen/memory.h"
#include <stdlib.h>
#include "../../include/CodeGen/object.h"
#include "../../include/VM/vm.h"

void	*reallocate(void *pointer, size_t oldSize, size_t newSize)
{
	void	*result;

	if (newSize == 0) return (free(pointer), NULL);
	result = realloc(pointer, newSize);
	if (!result) exit(1);
	return (result);
}

static void freeObject(Obj *object)
{
	switch (object->type)
	{
		/* per liberare la memeoria di una stringa */
		case OBJ_STRING:
		{
			ObjString	*string = (ObjString *)object;
			FREE_ARRAY(char, string->chars, string->lenght + 1);
			FREE(ObjString, object);
			break;
		}
	
	default:
		break;
	}
}

void	freeObjects()
{
	Obj	*object = vm.objects;
	while (object != NULL)
	{
		Obj	*next = object->next;
		freeObject(object);
		object = next;
	}
}