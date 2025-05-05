#include "../../include/CodeGen/common.h"
#include "../../include/CodeGen/chunk.h"
#include "../../include/CodeGen/debug.h"

int main(int argc, char **argv) 
{
	Chunk	chunk;
	int		constant;

	initChunk(&chunk);								// inizializza chunk
	constant = addConstant(&chunk, 2.3);			// aggiunge costante e ottiene indice
	writeChunk(&chunk, OP_CONSTANT);				// scrive opcode
	writeChunk(&chunk, constant);					// scrive indice della costante
	writeChunk(&chunk, OP_RETURN);					// scrive return
	disassembleChunk(&chunk, "test chunk");			// disassembla
	freeChunk(&chunk);								// libera memoria

	return (0);
}