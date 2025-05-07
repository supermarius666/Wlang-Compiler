#include "../../include/CodeGen/common.h"
#include "../../include/CodeGen/chunk.h"
#include "../../include/CodeGen/debug.h"
#include "../../include/VM/vm.h"

int main(int argc, char **argv) 
{
	initVm();										// inizializza Vm 
	Chunk	chunk;
	int		constant;

	/* -((2.3 + 1.43) / 5.6) */
	initChunk(&chunk);								// inizializza chunk
	constant = addConstant(&chunk, 2.3);			// aggiunge costante e ottiene indice
	writeChunk(&chunk, OP_CONSTANT, 1);				// scrive opcode
	writeChunk(&chunk, constant, 1);				// scrive indice della costante


	constant = addConstant(&chunk, 1.43);			// aggiunge costante e ottiene indice
	writeChunk(&chunk, OP_CONSTANT, 1);				// scrive opcode
	writeChunk(&chunk, constant, 1);				// scrive indice della costante

	writeChunk(&chunk, OP_ADD, 1);					// addizione 
	
	constant = addConstant(&chunk, 5.6);			// aggiunge costante e ottiene indice
	writeChunk(&chunk, OP_CONSTANT, 1);				// scrive opcode
	writeChunk(&chunk, constant, 1);				// scrive indice della costante

	writeChunk(&chunk, OP_DIV, 1);					// divisione 
	writeChunk(&chunk, OP_NEGATE, 1);				// negazione 


	writeChunk(&chunk, OP_RETURN, 1);				// scrive return
	disassembleChunk(&chunk, "test chunk");			// disassembla
	interpret(&chunk);								// esegue il codice, main entry point

	freeVm();										// libera Vm
	freeChunk(&chunk);								// libera memoria

	return (0);
}