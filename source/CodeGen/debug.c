#include "../../include/CodeGen/debug.h"
#include <stdio.h>


static int	simpleInstruction(const char *name, int offset)
{	
	printf("%s\n", name);
	return (offset + 1);
}

void	disassembleChunk(Chunk *chunk, const char *name)
{
	int	offset;
	
	printf("== %s ==\n", name);
	for (offset = 0; offset < chunk->count;)
		offset = disassembleInstruction(chunk, offset);
}

int		disassembleInstruction(Chunk *chunk, int offset)
{
	uint8_t	instruction;
	
	printf("%04d ", offset);
	instruction = chunk->code[offset];
	switch (instruction)
	{
	case OP_RETURN:
		return simpleInstruction("OP_RETURN", offset);
	
	default:
		printf("Unknown opcode %d\n", instruction);
		return (offset + 1);
	}
}