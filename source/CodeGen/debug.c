#include "../../include/CodeGen/debug.h"
#include "../../include/CodeGen/value.h"
#include <stdio.h>


static int	simpleInstruction(const char *name, int offset)
{	
	printf("%s\n", name);
	return (offset + 1);
}

static int constantInstruction(const char *name, Chunk *chunk, int offset)
{
	uint8_t	constant;

	constant = chunk->code[offset + 1];	/* get the index -> so che l'indice della costante si trova subito dopo opcode dell'istruzione costante */
	printf("%-16s %4d '", name, constant);
	printValue(chunk->constants.values[constant]);
	printf("'\n");
	return	(offset + 2); /* perché ho prima l'istruzione poi l'index della costante */
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
	
	if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
		printf("    | ");
	else
		printf("%4d ", chunk->lines[offset]);
	
	printf("%04d ", offset);
	instruction = chunk->code[offset];
	switch (instruction)
	{
		case OP_CONSTANT:
			return constantInstruction("OP_CONSTANT", chunk, offset);	
			
		case OP_NEGATE:
			return simpleInstruction("OP_NEGATE", offset);
		case OP_ADD:
			return simpleInstruction("OP_ADD", offset);
		case OP_SUB:
			return simpleInstruction("OP_SUB", offset);
		case OP_MUL:
			return simpleInstruction("OP_MUL", offset);
		case OP_DIV:
			return simpleInstruction("OP_DIV", offset);
		case OP_NIL:
			return simpleInstruction("OP_NIL", offset);
		case OP_TRUE:
			return simpleInstruction("OP_TRUE", offset);
		case OP_FALSE:
			return simpleInstruction("OP_FALSE", offset);
			return simpleInstruction("OP_TRUE", offset);
		case OP_NOT:
			return simpleInstruction("OP_NOT", offset);
		case OP_EQUAL:
			return simpleInstruction("OP_EQUAL", offset);
		case OP_GREATER:
			return simpleInstruction("OP_GREATER", offset);
		case OP_LESS:
			return simpleInstruction("OP_LESS", offset);
		case OP_PRINT:
			return simpleInstruction("OP_PRINT", offset);
		case OP_POP:
			return simpleInstruction("OP_POP", offset);
		case OP_DEFINE_GLOBAL:
			return constantInstruction("OP_DEFINE_GLOBAL", chunk, offset);
		case OP_GET_GLOBAL:
			return constantInstruction("OP_GET_GLOBAL", chunk, offset);
		case OP_SET_GLOBAL:
			return constantInstruction("OP_SET_GLOBAL", chunk, offset);
		
		case OP_RETURN:
			return simpleInstruction("OP_RETURN", offset);
		
		default:
			printf("Unknown opcode %d\n", instruction);
			return (offset + 1);
	}
}