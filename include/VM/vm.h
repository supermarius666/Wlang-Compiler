#ifndef WLANG_VM_H
# define WLANG_VM_H

# include "../CodeGen/chunk.h"
# include "../CodeGen/table.h"

/* numero massimo di elementi nella stack */
# define STACK_MAX 256

/* macchina virtuale */
typedef struct {
	Chunk	*chunk;				/* codice che deve eseguire in formato bytecode */
	uint8_t	*ip;				/* instruction pointer (ip) --> punta alla prossima istruzione da eseguire */
	Value	stack[STACK_MAX];	/* la stack della Vm*/
	Value	*stackTop;			/* stack pointer --> punta dove dovrebbe essere inserito il prossimo elemento */
	Table	strings;			/* hash table per tenere conto delle stringhe */
	Table	globals;			/* hash table per le variabili globali */
	Obj		*objects;			/* lista di oggetti allocati in modo dinamico */
}	Vm;

extern Vm vm;

/* esito dell'interpretazione codice quando lo eseguo! */
typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
}	InterpretResult;

/* init the Vm*/
void	initVm();

/* free the Vm*/
void	freeVm();

/* interpreta il codice passato alla Vm*/
InterpretResult	interpret(const char *source);

/* push sulla stack */
void	push(Value value);

/* pop dalla stack */
Value	pop();

#endif