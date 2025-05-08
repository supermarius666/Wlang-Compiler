#include "../../include/CodeGen/common.h"
#include "../../include/Lexer/scanner.h"
#include <stdio.h>
#include <string.h>

/* variabile globale scanner (lexer)*/
Scanner	scanner;

/* questa funzione mi dice se ho raggiunto la fine del codice sorgente */
static bool	isAtEnd() 
{
	return (*(scanner.current) == '\0');
}

/* avanza lo scanner al prossimo carattere e restituisce quello attuale */
static char	advance()
{
	scanner.current++;
	return (*(scanner.current - 1));
}

/* ritorna il carattere attuale senza avanzare lo scanner */
static char	peek()
{
	return (*scanner.current);
}

/* ritorna il carattere successivo a quello attuale --> lookahead */
static char peekNext()
{
	if (isAtEnd()) return ('\0');
	return (scanner.current[1]);
}

/* questa funzione verifica che il prossimo carattere sia quello expected e poi avanza nel codice */
static bool match(char expected)
{
	if (isAtEnd()) return (false);
	if (*(scanner.current) != expected) return (false);

	scanner.current++;
	return (true);
}

/* questa funzione costruisce e restituisce un token --> il token fa riferimento direttamente al valore presente nel codice sorgente */
static Token	makeToken(TokenType type)
{
	Token	token;

	token.type = type;
	token.line = scanner.line;
	token.start = scanner.start;
	token.lenght = (int)(scanner.current - scanner.start);
	return (token);
}

static void	skipWhiteSpace()
{
	while (true)
	{
		char c = peek();
		switch (c)
		{
		case ' ':
		case '\r':
		case '\t':
		{
			advance();
			break;
		}
		case '\n':
		{
			scanner.line++;
			advance();
			break;
		}

		/* commenti */
		case '/':
		{
			/* commento su singola linea --> i commenti non sono salvati come token, vengono ignorati */
			if (peekNext() == '/')
				while ((peek() != '\n') && (!isAtEnd())) advance();
			
			/* commento su più righe --> come su C */
			else if (peekNext() == '*') {
				advance(); // Consuma '/'
				advance(); // Consuma '*'
				while (!isAtEnd()) {
					if (peek() == '*' && peekNext() == '/') {
						advance(); // Consuma '*'
						advance(); // Consuma '/'
						break; // Esci dal ciclo se commento terminato
					}
					if (peek() == '\n') scanner.line++;
					
					advance();
				}
			}
			/* divisione */
			else
				return;
			break;
		}
		
		default:
			return;
		}	
	}
}

/* in caso di errore su un token viene generato un token di errore che contiene un messaggio di errore */
static Token	errorToken(const char *message)
{
	Token	token;

	token.type = ERROR;
	token.start = message;
	token.lenght = (int)strlen(message);
	token.line = scanner.line;

	return (token);
}

void	initScanner(const char *source)
{
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

Token	scanToken()
{
	skipWhiteSpace();
	scanner.start = scanner.current;

	if (isAtEnd()) return makeToken(EOF_TOKEN);

	char c = advance();
	switch (c)
	{	
		/* lexing di sigoli caratteri */
		case '(':return (makeToken(LEFT_PAREN));
		case ')': return (makeToken(RIGHT_PAREN));
		case '{': return (makeToken(LEFT_BRACE));
		case '}': return (makeToken(RIGHT_BRACE));
		case ';': return (makeToken(SEMICOLON));
		case ',': return (makeToken(COMMA));
		case '.': return (makeToken(DOT));
		case '+': return (makeToken(PLUS));
		case '-': return (makeToken(MINUS));
		case '*': return (makeToken(STAR));
		case '/': return (makeToken(SLASH));

		/* lexing di due caratteri */
		case '!':
		{
			if (match('!')) return (makeToken(END_STM));
			else if (match('=')) return (makeToken(NOT_EQUAL));
			else return (errorToken("Unexpected character."));
		}
		case '=':
		{
			return (makeToken(
				match('=') ? EQUALEQUAL : EQUAL
			));
		}
		case '<':
		{
			return (makeToken(
				match('=') ? LESS_EQUAL : LESS
			));
		}
		case '>':
		{
			return (makeToken(
				match('=') ? GREATER_EQUAL : GREATER
			));
		}
	}

	return errorToken("Unexpected character.");
}