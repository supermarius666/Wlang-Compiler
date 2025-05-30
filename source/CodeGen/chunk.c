#include "../../include/CodeGen/chunk.h"
#include <stdlib.h>

void	initChunk(Chunk *chunk)
{
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
	chunk->lines = NULL;
	initValueArray(&chunk->constants);
}

void	freeChunk(Chunk *chunk)
{
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	FREE_ARRAY(int, chunk->lines, chunk->capacity);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

void	writeChunk(Chunk *chunk, uint8_t byte, int line)
{
	int	oldCapacity;

	if (chunk->capacity < chunk->count + 1)
	{
		oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
		chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
	}
	chunk->code[chunk->count] = byte;
	chunk->lines[chunk->count] = line;
	chunk->count++;
}

int	addConstant(Chunk *chunk, Value value)
{
	writeValueArray(&chunk->constants, value);
	return (chunk->constants.count - 1);	/* return the index where the constat was appended */
}