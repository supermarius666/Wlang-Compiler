#include "../../include/VM/vm.h"
#include "../../include/VM/compiler.h"
#include "../../include/CodeGen/common.h"
#include "../../include/CodeGen/debug.h"
#include <stdio.h>

/* Vm come variabile globale */
Vm	vm;

/* mette stackTop all'inizio della stack --> indica che la stack è vuota e pronta per l'uso */
static void resetStack() 
{
	vm.stackTop = vm.stack;
}

void	initVm()
{
	resetStack();
}

void	freeVm()
{

}

/*
*	#define READ_BYTE() (*(vm.ip++))
*   questa macro legge l'istruzione puntata dall'ip e poi lo avanza alla prossima istruzione 
*
*	#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
*	questa macro prende l'indice della costante dall'istruzione puntata dall'ip e cerca la costante corrispondente nell'array di costanti associato al chunk
*
*	#define BINARY_OP(op) \
*	do { \
*		double b = pop(); \
*		double a = pop(); \
*		push(a op b); \
*	} while (false)
*	questa macro calcola le espressioni aritmetiche binarie 
*
*/

static InterpretResult run() 
{
# define READ_BYTE() (*(vm.ip++))	// faccio questa def qua perché serve solo qua, infatti, alla fine della funzione faccio undef
# define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
/* macro per calcolare le espressioni aritmetiche */
# define BINARY_OP(op) \
	do { \
		double b = pop(); \
		double a = pop(); \
		push(a op b); \
	} while (false)

	while (true) {
#ifdef DEBUG_TRACE_EXECUTION
		printf("				");
		for (Value *slot = vm.stack; slot < vm.stackTop; slot++)
		{
			printf("[ ");
			printValue(*slot);
			printf(" ]");
		}
		printf("\n");
		/* in debug mode mi stampa il trace delle istruzioni --> (vm.ip - vm.chunk->code) calcola l'offset dall'inizio del chunk */
		// disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
		uint8_t	instruction;
		switch (instruction = READ_BYTE())
		{
			case OP_CONSTANT:
			{
				Value costant = READ_CONSTANT();
				push(costant);
				break;
			}

			case OP_ADD: BINARY_OP(+); break;
			case OP_SUB: BINARY_OP(-); break;
			case OP_DIV: BINARY_OP(/); break;
			case OP_MUL: BINARY_OP(*); break;
			case OP_NEGATE: push(-pop()); break;	/* faccio la negazione dell'numero poi lo pusho sulla stack */

			case OP_RETURN:
			{
				printValue(pop());
				printf("\n");
				return INTERPRET_OK;
			}

			default:
				break;
		}
	}
# undef BINARY_OP
# undef READ_CONSTANT
# undef READ_BYTE
}

InterpretResult	interpret(const char *source)
{
	compile(source);
	return INTERPRET_OK;
}

void	push(Value value)
{
	*(vm.stackTop) = value;		/* salvo il valore da pushare nello spazio puntato dal SP*/
	vm.stackTop++;				/* incremento SP*/
}

Value	pop()
{
	vm.stackTop--;				/* decremento SP*/
	return *(vm.stackTop);		/* mi prendo il valore */
}