#ifndef WLANG_PARSER_H
# define WLANG_PARSER_H

#include "../Lexer/scanner.h"
#include <stdbool.h>


/* parser */
typedef struct
{
	Token	current;
	Token	previous;
	bool	hadError;	/* se c'è un errore durante il parsing diventa true e la funzione compile ritorna false */
	bool	panicMode; /* flag per evitare di generare errori di parsing in cascata --> quando ho un token sbagliato il parser potrebbe non sapere più do
						  dove si trova nel codice e quindi tutto quello che viene dopo è un errore di parsing, lo evito con questa flag */
}	Parser;

/* precedenza */
typedef enum
{
	PREC_NONE,
	PREC_ASSIGNMENT,	// =
	PREC_OR,   			// o
	PREC_AND,			// e
	PREC_EQUAL,			// ==, !=
	PREC_CMP,		  	// <, >, <=, >=
	PREC_TERM,         	// +, -
	PREC_FACTOR,	   	// *, /
	PREC_UNARY,        	// non, -
	PREC_CALL,		  	// ., ()
	PREC_PRIMARY,
}	Precedence;

/* puntatore a funzione */
typedef void (*ParseFn)(bool canAssign);

/* parsing rules */
typedef struct 
{
	ParseFn		prefix;
	ParseFn		infix;
	Precedence	precedance;
}	ParseRule;


#endif