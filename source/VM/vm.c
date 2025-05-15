#include "../../include/VM/vm.h"
#include "../../include/VM/compiler.h"
#include "../../include/CodeGen/common.h"
#include "../../include/CodeGen/debug.h"
#include <stdio.h>
#include <stdarg.h>

/* Vm come variabile globale */
Vm	vm;

/* mette stackTop all'inizio della stack --> indica che la stack è vuota e pronta per l'uso */
static void resetStack() 
{
	vm.stackTop = vm.stack;
}

/* ritorna il valore dalla stack a distance dal dalla testa --> 0 significa elemento in testa */
static Value	peek(int distance)
{
	return vm.stackTop[-1 - distance ];
}

/* questa funzione verifica se una variabile è nulla o è booleana falsa --> allora è falsey; tutto il resto è true(vero)*/
static bool isFalsey(Value value)
{
	return( (IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value))));
}

/* funzione che stampa l'errore e ferma il compilatore */
static void runtimeError(const char *format, ...)
{
	va_list	args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputs("\n", stderr);

	size_t instruction = vm.ip - vm.chunk->code - 1;
	int line = vm.chunk->lines[instruction];
	fprintf(stderr, "[line %d] in %s\n", line, input_path);

	resetStack();
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
# define BINARY_OP(valueType, op) \
	do { \
		if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
			runtimeError("Operands must be numbers.");	\
			return INTERPRET_RUNTIME_ERROR;	\
		} \
		double b = AS_NUMBER(pop()); 	\
		double a = AS_NUMBER(pop()); 	\
		push(valueType(a op b)); 		\
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

			case OP_ADD: BINARY_OP(NUMBER_VAL, +); break;
			case OP_SUB: BINARY_OP(NUMBER_VAL, -); break;
			case OP_DIV: BINARY_OP(NUMBER_VAL, /); break;
			case OP_MUL: BINARY_OP(NUMBER_VAL, *); break;
			case OP_NEGATE:
			{
				/* validazione: verifico che in ho un numero in testa alla stack --> se non è un numero genero errore e fermo tutto */
				if (!IS_NUMBER(peek(0)))
				{
					runtimeError("Operand must be a number.");
					return INTERPRET_RUNTIME_ERROR;
				}
				push(NUMBER_VAL(-AS_NUMBER(pop())));
				break;
			}
			
			/* literals */
			case OP_NIL: push(NIL_VAL); break;
			case OP_TRUE: push(BOOL_VAL(true)); break;
			case OP_FALSE: push(BOOL_VAL(false)); break;

			case OP_NOT:
			{
				/* faccio pop dell'operando, poi lo nego e poi lo ripusho sulla stack */
				push(BOOL_VAL(isFalsey(pop()))); break;
			}
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

/* questa funzione chiama il compilatore che si occupa di generare il bytecode */
InterpretResult	interpret(const char *source)
{
	Chunk	chunk;

	initChunk(&chunk);
	if (!compile(source, &chunk))
	{
		freeChunk(&chunk);
		return INTERPRET_COMPILE_ERROR;	
	}

	/* passo il chunk di bytecode generato dal compiler alla vm per essere eseguit0 --> in futuro vm e compilatore saranno cose diverse*/
	vm.chunk = &chunk;
	vm.ip = vm.chunk->code;

	InterpretResult result = run();

	freeChunk(&chunk);
	return result;

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