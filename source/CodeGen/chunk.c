#include "../../include/CodeGen/chunk.h"
#include <stdlib.h>

void	initChunk(Chunk *chunk)
{
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
}

void	freeChunk(Chunk *chunk)
{
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	initChunk(chunk);
}

void	writeChunk(Chunk *chunk, uint8_t byte)
{
	int	oldCapacity;

	if (chunk->capacity < chunk->count + 1)
	{
		oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
	}
	chunk->code[chunk->count] = byte;
	chunk->count++;
}