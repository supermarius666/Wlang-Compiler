#ifndef WLANG_DEBUG_H
# define WLANG_DEBUG_H

# include "common.h"
# include "chunk.h"


/**
 * @brief Disassembles a chunk of bytecode for debugging purposes.
 * 
 * This function takes a chunk of bytecode and a name, and outputs a 
 * human-readable disassembly of the chunk to aid in debugging.
 * 
 * @param chunk Pointer to the Chunk object containing the bytecode to disassemble.
 * @param name A descriptive name for the chunk, typically used as a label in the output.
 */
void	disassembleChunk(Chunk *chunk, const char *name);

/**
 * @brief Disassembles a single instruction from the given chunk and prints its details.
 * 
 * This function is used for debugging purposes to analyze the bytecode instructions
 * in a chunk. It prints the disassembled instruction to the standard output, along
 * with its offset and any associated operands.
 * 
 * @param chunk A pointer to the Chunk object containing the bytecode instructions.
 * @param offset The offset of the instruction to disassemble within the chunk.
 * @return The offset of the next instruction after the disassembled one.
 */
int		disassembleInstruction(Chunk *chunk, int offset);

#endif