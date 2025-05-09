#ifndef WLANG_SCANNER_H
# define WLANG_SCANNER_H

/* scanner che fa il lexing del codice sorgente  */
typedef struct {
	const char	*start;
	const char	*current;
	int			line;
}	Scanner;

typedef enum{
    /* Literals */
	IDENTIFIER, NUMBER, STRING,

	/* Keywords */
	SIA, STAMPA, SE, ALTRIMENTI, MENTRE, NULLA,
	E, O, NON, VERO, FALSO, MAIN, FUN, RETURN, 

	/* Arithmetic operators */
	PLUS, MINUS, STAR, SLASH, EQUAL, NOT_EQUAL,
	GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

	/* Punctuation, Parentheses and Others */
	SEMICOLON, COMMA, DOT, EQUALEQUAL,
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	END_STM, ERROR,

	/* End of file */
	EOF_TOKEN
}	TokenType;

typedef struct {
	TokenType	type;		/* tipo token */
	const char	*start;		/* inizio token */
	int			lenght;		/* lunghezza token */
	int			line;		/* linea sulla quale si trova il token */
}	Token;

/* inizializza lo scanner */
void	initScanner(const char *source);

/* fa lo scan di un token alla volta e usa direttamente il codice sorgente senza allocare nulla --> i token sono passati per valore */
Token	scanToken();
#endif