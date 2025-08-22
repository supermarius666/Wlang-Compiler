#ifndef WLANG_CHUNK_H
# define WLANG_CHUNK_H

# include "common.h"
# include "memory.h"
# include "value.h"


/**
 * @enum
 * @brief Represents the different types of values or instructions that can be stored in a chunk.
 * 
 * This enumeration is used to define the various categories or types that are
 * utilized during code generation in the Wlang compiler.
 */
typedef enum {
	OP_CONSTANT,			/* for constant instructions like print (stampa)  --> takes a single byte operand that specifies which constant to load from the chunk's constant array*/
	OP_NEGATE,				/* negazione di un numero --> operatore unario */
	OP_NOT,					/* il not logico, per invertire roba booleana */
	OP_ADD,					/* somma */
	OP_SUB,					/* sottrazione */
	OP_MUL,					/* moltiplicazione */
	OP_DIV,					/* divisione */

	/* servono solo tre operatori per fare <, >, <=, >=, ==, != */
	OP_EQUAL,				/* uguale */
	OP_GREATER,				/* maggiore */
	OP_LESS,				/* minore */

	OP_NIL,					/* per valori nulli */
	OP_TRUE,				/* per valore vero*/
	OP_FALSE,				/* per valore falso */
	OP_POP,					/* per un expression stmt --> pop dalla stack */

	/* variabili */
	OP_DEFINE_GLOBAL,		/* per le variabili globali */
	OP_GET_GLOBAL,			/* per caricare una variabile globale */
	OP_SET_GLOBAL,			/* per assegnare una variabile globale */
	OP_GET_LOCAL,			/* Variabili locali */
	OP_SET_LOCAL,

	OP_PRINT,				/* per stampa */
	OP_JUMP_IF_FALSE,		/* per il branch false nell'if */
	OP_JUMP,				/* jump normale */
	OP_LOOP,				/* per i loop*/
	OP_RETURN,				/* return code */

}	OpCode;


/**
 * @brief Represents a chunk of memory or data used in the code generation process.
 * 
 * This structure is used to store and manage a sequence of instructions
 * or data for the compiler's code generation phase.
 */
typedef struct {
	int			count;		/* how many spots are in use */
	int			capacity;	/* capacity of the dynamic array */
	uint8_t		*code;		/* array of bytes */
	int			*lines;		/*line numers*/
	ValueArray	constants;	/* constants values (literals)*/
}	Chunk;

/* methods */


/**
 * @brief Initializes a Chunk structure.
 * 
 * This function sets up the initial state of a Chunk object, preparing it
 * for use. It ensures that all necessary fields are properly initialized.
 * 
 * @param chunk A pointer to the Chunk structure to be initialized.
 */
void	initChunk(Chunk *chunk);

/**
 * @brief Frees the memory allocated for a Chunk.
 *
 * This function releases all resources associated with the given Chunk,
 * including any dynamically allocated memory within the Chunk structure.
 *
 * @param chunk A pointer to the Chunk to be freed. The pointer must not be NULL.
 */
void	freeChunk(Chunk *chunk);

/**
 * @brief Writes a single byte to the specified chunk.
 *
 * This function appends a byte to the end of the given chunk, expanding
 * its capacity if necessary.
 *
 * @param chunk A pointer to the Chunk structure where the byte will be written.
 * @param byte The byte value to be written to the chunk.
 */
void	writeChunk(Chunk *chunk, uint8_t byte, int line);

int		addConstant(Chunk *chunk, Value value);

#endif