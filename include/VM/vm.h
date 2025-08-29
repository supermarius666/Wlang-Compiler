#ifndef WLANG_VM_H
# define WLANG_VM_H

# include "../CodeGen/chunk.h"
# include "../CodeGen/table.h"
# include "../CodeGen/object.h"


/* numero massimo di chiamate a funzione */
# define FRAME_MAX 64


/* numero massimo di elementi nella stack */
# define STACK_MAX (FRAME_MAX * UINT8_COUNT)

/* call frame per le funzioni  */
typedef struct 
{
	ObjFunction	*function;
	uint8_t		*ip;
	Value		*slots;
}	CallFrame;


/* macchina virtuale */
typedef struct {
	CallFrame	frames[FRAME_MAX];	/* stack per i frame di attivazione delle funzioni */
	int 		frameCount;			/* numero di frame nella stack */
	Value		stack[STACK_MAX];	/* la stack della Vm*/
	Value		*stackTop;			/* stack pointer --> punta dove dovrebbe essere inserito il prossimo elemento */
	Table		strings;			/* hash table per tenere conto delle stringhe */
	Table		globals;			/* hash table per le variabili globali */
	Obj			*objects;			/* lista di oggetti allocati in modo dinamico */
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

/* per codice da .wlb*/
InterpretResult runCompiled(ObjFunction* function);

/* push sulla stack */
void	push(Value value);

/* pop dalla stack */
Value	pop();

#endif