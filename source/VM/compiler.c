#include "../../include/CodeGen/common.h"
#include "../../include/VM/compiler.h"
#include "../../include/Lexer/scanner.h"
#include "../../include/Parser/parser.h"
#include "../../include/CodeGen/object.h"
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
static void	binary(bool canAssign);
static void	literal(bool canAssign);
static bool match(TokenType type);
static void defineVariable(uint8_t global);
static uint8_t makeCostant(Value value);

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

/* ritorna true se il token è del tipo giusto */
static bool check(TokenType type)
{
	return (parser.current.type == type);
}

/* funzione che verifica se il token che mi aspetto è quello giusto */
static bool match(TokenType type)
{
	if (!check(type)) return (false);
	advance();
	return (true);
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

static void printStatement()
{
	consume(LEFT_PAREN, "Expected '(' after 'stampa'.");
    expression();

	if (match(END_STM)) 
	{
		/* da vedere --> questo controllo nasce da una roba che è capitata mentre testavo il codice */
        errorAtCurrent("Unexpected '!!' before closing ')'. Did you mean: stampa(expr)!! ?");
        return;
    }

    consume(RIGHT_PAREN, "Expected ')' after expression.");
    consume(END_STM, "Expected '!!' after expression.");
    emitByte(OP_PRINT);
}

/*	funzione per sincronizzare il parser quando va in panicMode 
*	salto i token finche non trovo un punto che denota la fine di un stmt e
*	quindi posso sincronizzare il parser
*/
static void synchronize()
{
	parser.panicMode = false;

	while (parser.current.type != EOF_TOKEN)
	{
		if (parser.previous.type == END_STM) return;

		switch (parser.current.type)
		{
			/* potrei aggiungere class per le classi */
			case SE:
			case SIA:	/* dichiarazione variabili */
			case STAMPA:
			case MENTRE:
			case FUN:
			case RETURN:
				return;
		
		default:
		/* non deve fare nulla qua */
			;
		}

		advance();
	}
}

/* funzione per il parsing degli expression stmt --> espressione seguita da !! */
static void expressionStatement()
{
	expression();
	consume(END_STM, "Expected '!!' after expression.");
	emitByte(OP_POP);
}



/*	prende il token identifier e aggiunge il suo lexema, cioè il nome della variabile
*	nella lista delle costanti
*/
static uint8_t	identifierConstant(Token *name)
{
	return (makeCostant(OBJ_VAL(copyString(name->start, name->lenght))));
}

/* funzione per il parsing della variabile */
static uint8_t parseVariable(const char *errorMessage)
{
	consume(IDENTIFIER, errorMessage);
	return (identifierConstant(&parser.previous));
}

/* funzione per la dichiarazione di variabili */
static void varDeclaration()
{
	uint8_t	global = parseVariable("Expected variable name.");

	/* se c'è l'uguale prendo l'espressione */
	if (match(EQUAL))
		expression();
	/* altrimenti la variabile va messa a 'nulla' */
	else
		emitByte(OP_NIL);
	
	consume(END_STM, "Expected '!!' after expression.");
	
	/* definisco la variabile globale */
	defineVariable(global);
}

/* funzione per fare parsing degli stmt */
static void statement()
{
	if (match(STAMPA))
		printStatement();
	else
		expressionStatement();
}

/* funzione che si occupa degli stmt di dichiarazione */
static void declaration()
{
	if (match(SIA))
		varDeclaration();	/* per dichiarazioni di variabile con 'sia' */
	else
		statement();

	if (parser.panicMode) synchronize();
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

/*	questa funzione genera il bytecode per la variabile globale il cui nome è salvato nella
*	lista delle costanti alla quale posso fare riferimento con l'indice nella lista restituito da
*	identifierConstant. il valore è salvato anche (global)
*/
static void defineVariable(uint8_t global)
{
	emitBytes(OP_DEFINE_GLOBAL, global);
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
static void	number(bool canAssign)
{
	double value = strtod(parser.previous.start, NULL);
	emitCostant(NUMBER_VAL(value));
}

/* */
static void	string(bool canAssign)
{
	/*	prende la stringa direttamente dal lexema. Il +1 e -2 servono a togliere gli apici
		dopo viene creato un oggetto string, messo in value e poi messo nella lista delle costanti 
	*/
	emitCostant(OBJ_VAL(copyString(parser.previous.start + 1, parser.previous.lenght - 2)));
}

/* grouping --> cose tra parentesi;  prefix expression --> significa che ha un token particolare con il quale inizia */
static void	grouping(bool canAssign)
{
	expression();
	consume(RIGHT_PAREN, "Expected ')' after expression.");
}

/* negazione --> prefix expression */
static void unary(bool canAssign)
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

/*	questa funzione prende il nome della variabile, poi trova l'indice nella lista della costanti
*	e poi genera il bytecode per per caricare la variabile globale
*/
static void nameVariable(Token name, bool canAssign)
{
	uint8_t arg = identifierConstant(&name);
	
	if (canAssign &&  match(EQUAL))
	{
		expression();
		emitBytes(OP_SET_GLOBAL, arg);
	}
	else
		emitBytes(OP_GET_GLOBAL, arg);
}

/* questo è per accedere alla variabile tramite il nome */
static void variable(bool canAssign)
{
	nameVariable(parser.previous, canAssign);
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
	[NOT_EQUAL]				= 	{NULL, binary, PREC_EQUAL},
	[EQUAL]					= 	{NULL, NULL, PREC_NONE},
	[EQUALEQUAL]			= 	{NULL, binary, PREC_EQUAL},
	[GREATER]				= 	{NULL, binary, PREC_CMP},
	[GREATER_EQUAL]			= 	{NULL, binary, PREC_CMP},
	[LESS]					= 	{NULL, binary, PREC_CMP},
	[LESS_EQUAL]			= 	{NULL, binary, PREC_CMP},
	[IDENTIFIER]			= 	{variable, NULL, PREC_NONE},
	[STRING]				= 	{string, NULL, PREC_NONE},
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
static void	binary(bool canAssign)
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
		
		/* ==
		* >
		* <
		* (a != b) --> !(a == b)
		* (a <= b) --> !(a > b)
		* (a >= b) --> !(a < b) 
		*/
		case NOT_EQUAL: emitBytes(OP_EQUAL, OP_NOT); break;
		case EQUALEQUAL: emitByte(OP_EQUAL); break;
		case GREATER: emitByte(OP_GREATER); break;
		case GREATER_EQUAL: emitBytes(OP_LESS, OP_NOT); break;
		case LESS: emitByte(OP_LESS); break;
		case LESS_EQUAL: emitBytes(OP_GREATER, OP_NOT); break;

		default:
			return;
	}

}

/* funzione per i literals */
static void	literal(bool canAssign)
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
	bool canAssign = precedence <= PREC_ASSIGNMENT;
	prefixRule(canAssign);

	while (precedence <= getRule(parser.current.type)->precedance)
	{
		advance();
		ParseFn infixRule = getRule(parser.previous.type)->infix;
		infixRule(canAssign);
	}

	if (canAssign && match(EQUAL))
		error("Invalid assignment target.");
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
	
	while (!match(EOF_TOKEN))
		declaration();

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
