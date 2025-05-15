#include "../../include/CodeGen/common.h"
#include "../../include/VM/compiler.h"
#include "../../include/Lexer/scanner.h"
#include "../../include/Parser/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# ifdef DEBUG_PRINT_CODE
# include "../../include/CodeGen/debug.h"
#endif

/* variabile globale del parser --> verifica la grammatica del linguaggio */
Parser	parser;

/* chunk attuale che sto compilando */
Chunk	*compilingChunk;

/* forward declaration */
static void	parsePrecedence(Precedence precedence);
static void expression();
static void	binary();
static void	literal();


/* ritorna il chunk attuale */
static Chunk	*currentChunk()
{
	return (compilingChunk);
}

/* funzione che gestisce l'errore */
static void	errorAt(Token *token, const char *message)
{
	if (parser.panicMode) return;
	parser.panicMode = true;	/* se è attivo --> tutti gli altri errori successivi sono ignorati */

	fprintf(stderr, "[Line %d] Error", token->line);

	if (token->type == EOF) fprintf(stderr, " at end");
	else if (token->type == ERROR) { /* nulla */ }
	else fprintf(stderr, " at '%.*s'", token->lenght, token->start);
	
	fprintf(stderr, ": %s\n", message);
	parser.hadError = true;
}

/* segnala un errore su un token che è già stato consumato */
static void error(const char *message)
{
	errorAt(&parser.previous, message);
}

/* segnala errore con un messaggio sul token attuale */
static void	errorAtCurrent(const char *message)
{
	/* prendo la poszione attuale di dive si trova l'errore --> prendo la posizione del token */
	errorAt(&parser.current, message);
}

/* legge il prossimo token */
static void	advance()
{
	parser.previous = parser.current;

	while (true)
	{
		parser.current = scanToken();
		if (parser.current.type != ERROR) break;

		errorAtCurrent(parser.current.start);
	}
}
/* vede se il token che sta per consumare ha il valore aspettato e poi avanza, altrimenti errore */
static void	consume(TokenType type, const char *message)
{
	if (parser.current.type == type)
	{
		advance();
		return;
	}
	errorAtCurrent(message);
}

/* agginge un byte al chunk di byte */
static void	emitByte(uint8_t byte)
{
	writeChunk(currentChunk(), byte, parser.previous.line);
}

/* questa funzione aggiunge il return alla fine della compilazione del chunk */
static void emitReturn()
{
	emitByte(OP_RETURN);
}

/* valuta le espressioni */
static void	expression()
{
	parsePrecedence(PREC_ASSIGNMENT);
}

/* mi da l'indice della costante nella lista di costanti associate al chunk */
static uint8_t makeCostant(Value value)
{
	int costant = addConstant(currentChunk(), value);
	if (costant > UINT8_MAX)  /* in futuro si potrebbe espandere --> posso avere 256 costanti per chunk */
	{
		error("Too many costants in one chunk");
		return (0);
	}
	return ((uint8_t)costant);
}


/* utile per le istruzioni che hanno un argomento di un byte */
static void	emitBytes(uint8_t byte1, uint8_t byte2)
{
	emitByte(byte1);
	emitByte(byte2);
	return;
}

/* mette la l'istruzione costante e il valore costante nel chunk */
static void	emitCostant(Value value)
{
	emitBytes(OP_CONSTANT, makeCostant(value));
}

/* questo chiude la compilazione con return */
static void	endCompiler()
{
	emitReturn();
	#ifdef DEBUG_PRINT_CODE
		if (!parser.hadError) disassembleChunk(currentChunk(), "code");
	#endif
}

/* prende il lexema del literale number */
static void	number()
{
	double value = strtod(parser.previous.start, NULL);
	emitCostant(NUMBER_VAL(value));
}

/* grouping --> cose tra parentesi;  prefix expression --> significa che ha un token particolare con il quale inizia */
static void	grouping()
{
	expression();
	consume(RIGHT_PAREN, "Expected ')' after expression.");
}

/* negazione --> prefix expression */
static void unary()
{
	TokenType opType = parser.previous.type;

	// compilazione opratore 
	parsePrecedence(PREC_UNARY);

	// bytecode 
	switch (opType)
	{
		/* nagazione numero con meno */
		case MINUS: emitByte(OP_NEGATE); break;

		/* negazione booleano con non */
		case NON:	emitByte(OP_NOT); break;
		
		default:
			return;
	}
}

/* questa è la tabella dei puntatori a funzione per ogni tipo di espressione */
ParseRule rules []= {
	[LEFT_PAREN]			=	{grouping, NULL, PREC_NONE}, 
	[RIGHT_PAREN]			=	{NULL, NULL, PREC_NONE}, 
	[LEFT_BRACE]			=	{NULL, NULL, PREC_NONE}, 
	[RIGHT_BRACE]			=	{NULL, NULL, PREC_NONE}, 
	[COMMA]					=	{NULL, NULL, PREC_NONE}, 
	[DOT]					=	{NULL, NULL, PREC_NONE}, 

	[MINUS]					=	{unary, binary, PREC_TERM},
	[PLUS]					=	{NULL, binary, PREC_TERM},
	[SEMICOLON]				=	{NULL, NULL, PREC_TERM},


	[SLASH]					=	{NULL, binary, PREC_FACTOR},
	[STAR]					=	{NULL, binary, PREC_FACTOR},

	[NON]					= 	{unary, NULL, PREC_NONE},
	[NOT_EQUAL]				= 	{NULL, NULL, PREC_NONE},
	[EQUAL]					= 	{NULL, NULL, PREC_NONE},
	[EQUALEQUAL]			= 	{NULL, NULL, PREC_NONE},
	[GREATER]				= 	{NULL, NULL, PREC_NONE},
	[GREATER_EQUAL]			= 	{NULL, NULL, PREC_NONE},
	[LESS]					= 	{NULL, NULL, PREC_NONE},
	[LESS_EQUAL]			= 	{NULL, NULL, PREC_NONE},
	[IDENTIFIER]			= 	{NULL, NULL, PREC_NONE},
	[STRING]				= 	{NULL, NULL, PREC_NONE},
	[NUMBER]				=	{number, NULL, PREC_NONE},

	[E]						=	{NULL, NULL, PREC_NONE},
	[ALTRIMENTI]			=	{NULL, NULL, PREC_NONE},
	[SE]					=	{NULL, NULL, PREC_NONE},
	[VERO]					=	{literal, NULL, PREC_NONE},
	[FALSO]					=	{literal, NULL, PREC_NONE},
	[STAMPA]				=	{NULL, NULL, PREC_NONE},
	[O]						=	{NULL, NULL, PREC_NONE},
	[SE]					=	{NULL, NULL, PREC_NONE},
	[FUN]					=	{NULL, NULL, PREC_NONE},

	[ERROR]					=	{NULL, NULL, PREC_NONE},
	[EOF_TOKEN]				=	{NULL, NULL, PREC_NONE},
	[END_STM]				=	{NULL, NULL, PREC_NONE},
	[MENTRE]				=	{NULL, NULL, PREC_NONE},
	[RETURN]				=	{NULL, NULL, PREC_NONE},
	[NULLA]					=	{literal, NULL, PREC_NONE},
};

/* prende la regola giuta in base al token */
static ParseRule	*getRule(TokenType type)
{
	return (&rules[type]);
}

/* espressioni binari --> espressione infix --> operatore in mezzo */
static void	binary()
{
	TokenType opType = parser.previous.type;

	/* prendo la regola giusta per il parsing dalla tabella delle regole */
	ParseRule *rule = getRule(opType);
	parsePrecedence((Precedence)(rule->precedance + 1));

	/* genero il bytecode dell'operatore */
	switch (opType)
	{
		case PLUS: emitByte(OP_ADD); break;
		case MINUS: emitByte(OP_SUB); break;
		case STAR: emitByte(OP_MUL); break;
		case SLASH: emitByte(OP_DIV); break;	
		
		default:
			return;
	}

}

/* funzione per i literals */
static void	literal()
{
	switch (parser.previous.type)
	{
		case VERO: emitByte(OP_TRUE); break;
		case FALSO: emitByte(OP_FALSE); break;
		case NULLA: emitByte(OP_NIL); break;
	default:
		break;
	}
}

/* la funzione che gestisce tutte le funzioni del parser */
static void	parsePrecedence(Precedence precedence)
{
	advance();
	ParseFn prefixRule = getRule(parser.previous.type)->prefix;
	if (prefixRule == NULL) 
	{
		error("Expect expression.");
		return;
	}
	prefixRule();

	while (precedence <= getRule(parser.current.type)->precedance)
	{
		advance();
		ParseFn infixRule = getRule(parser.previous.type)->infix;
		infixRule();
	}
}

bool	compile(const char *source, Chunk *chunk)
{
	/* innit dello scanner che produce token */
	initScanner(source);

	/* init del chunk attuale */
	compilingChunk = chunk;

	parser.hadError = false;
	parser.panicMode = false;

	advance();
	expression();
	consume(EOF_TOKEN, "Expected end of expression");
	endCompiler();
	return (!parser.hadError);
}


/* utils */

static const char *tokenTypeToString(TokenType type)
{
	switch (type)
	{
		case IDENTIFIER: return "IDENTIFIER";
		case NUMBER: return "NUMBER";
		case STRING: return "STRING";

		case SIA: return "SIA";
		case STAMPA: return "STAMPA";
		case SE: return "SE";
		case ALTRIMENTI: return "ALTRIMENTI";
		case E: return "E";
		case O: return "O";
		case NON: return "NON";
		case VERO: return "VERO";
		case FALSO: return "FALSO";
		case MAIN: return "MAIN";
		case FUN: return "FUN";
		case MENTRE: return "MENTRE";
		case NULLA: return "NULLA";
		case RETURN: return "RETURN";

		case PLUS: return "PLUS";
		case MINUS: return "MINUS";
		case STAR: return "STAR";
		case SLASH: return "SLASH";
		case EQUAL: return "EQUAL";
		case NOT_EQUAL: return "NOT_EQUAL";
		case GREATER: return "GREATER";
		case GREATER_EQUAL: return "GREATER_EQUAL";
		case LESS: return "LESS";
		case LESS_EQUAL: return "LESS_EQUAL";

		case SEMICOLON: return "SEMICOLON";
		case COMMA: return "COMMA";
		case DOT: return "DOT";
		case EQUALEQUAL: return "EQUALEQUAL";
		case LEFT_PAREN: return "LEFT_PAREN";
		case RIGHT_PAREN: return "RIGHT_PAREN";
		case LEFT_BRACE: return "LEFT_BRACE";
		case RIGHT_BRACE: return "RIGHT_BRACE";
		case END_STM: return "END_STM";
		case ERROR: return "ERROR";

		case EOF_TOKEN: return "EOF_TOKEN";

		default: return "UNKNOWN_TOKEN";
	}
}
