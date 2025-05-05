#include "../../include/CodeGen/memory.h"
#include <stdlib.h>

void	*reallocate(void *pointer, size_t oldSize, size_t newSize)
{
	void	*result;

	if (newSize == 0) return (free(pointer), NULL);
	result = realloc(pointer, newSize);
	if (!result) exit(1);
	return (result);
}