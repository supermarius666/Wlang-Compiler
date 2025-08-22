#include "../../include/VM/vm.h"
#include "../../include/VM/compiler.h"
#include "../../include/CodeGen/common.h"
#include "../../include/CodeGen/debug.h"
#include <stdio.h>
#include <string.h>
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

/* funzione che concatena due stringhe */
static void concatenate()
{
	ObjString	*b = AS_STRING(pop());
	ObjString	*a = AS_STRING(pop());

	int len = a->lenght + b->lenght;
	char *chars = ALLOCATE(char, len + 1);
	
	memcpy(chars, a->chars, a->lenght);
	memcpy(chars + a->lenght, b->chars, b->lenght);
	chars[len] = '\0';

	ObjString	*result = takeString(chars, len);
	push(OBJ_VAL(result));
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
	vm.objects = NULL;
	initTable(&vm.globals);
	initTable(&vm.strings);
}

void	freeVm()
{
	freeTable(&vm.globals);
	freeTable(&vm.strings);
	freeObjects();
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
# define READ_STRING()	AS_STRING(READ_CONSTANT())
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

			/* l'operatore + funziona per i numeri e fa la somma e per le strighe dove fa la concatenazione */
			case OP_ADD: 
			{
				if (IS_STRING(peek(0)) && IS_STRING(peek(1)))
					concatenate();
				else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1)))
				{
					double b = AS_NUMBER(pop());
					double a = AS_NUMBER(pop());
					push(NUMBER_VAL(a + b));
				}
				else
				{
					runtimeError("Operands must be two numbers or two strings.");
					return (INTERPRET_RUNTIME_ERROR);
				}
				break;
			}
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

			/* comparazione--> cmp */
			case OP_EQUAL:
			{
				Value a = pop();
				Value b = pop();
				/* posso usare == su tante coppie di oggetti, per gestire la logica uso la funzione valuesEqual*/
				push(BOOL_VAL(valuesEqual(a, b)));
				break;
			}

			case OP_GREATER: BINARY_OP(BOOL_VAL, >); break;
			case OP_LESS: BINARY_OP(BOOL_VAL, <); break;

			case OP_POP: pop(); break;

			/* per stampare */
			case OP_PRINT:
			{
				printValue(pop());
				printf("\n");  //per avere newline tra chiamate successive di stampa
				break;
			}

			/* definizione variabili */
			case OP_DEFINE_GLOBAL:
			{
				ObjString* name = READ_STRING();

				// aggiungo la stringa nella hash table
				tableSet(&vm.globals, name, peek(0));
				
				pop(); // Rimuove il valore dallo stack
				break;
			}

			/* prendo il valore di una variabile */
			case OP_GET_GLOBAL:
			{
				ObjString* name = READ_STRING();
				Value value;

				if (!tableGet(&vm.globals, name, &value))
				{
					runtimeError("Undefined variable '%s'.", name->chars);
					return (INTERPRET_RUNTIME_ERROR);
				}

				push(value);
				break;
			}

			case OP_SET_GLOBAL:
			{
				ObjString* name = READ_STRING();
				
				if (tableSet(&vm.globals, name, peek(0)))
				{
					tableDelete(&vm.globals, name);
					runtimeError("Cannot assign to undefined variable '%s'.", name->chars);
					return (INTERPRET_RUNTIME_ERROR);
				}
				
				break;
			}

			case OP_GET_LOCAL:
			{
				uint8_t slot = READ_BYTE();
				push(vm.stack[slot]);
				break;
			}

			case OP_SET_LOCAL:
			{
				uint8_t slot = READ_BYTE();
				vm.stack[slot] = peek(0);
				break;
			}

			case OP_RETURN:
			{
				return INTERPRET_OK;
			}

			default:
				break;
		}
	}
# undef BINARY_OP
# undef READ_STRING
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