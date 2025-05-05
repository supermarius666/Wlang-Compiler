#ifndef WLANG_TOKEN_HPP
#define WLANG_TOKEN_HPP

#include <string>

/* enum for all token types*/
enum class TokenType {
    /* Literals */
	IDENTIFIER, NUMBER, STRING,

	/* Keywords */
	SIA, STAMPA, SE, ALTRIMENTI,
	E, O, NON, VERO, FALSO, MAIN, FUN,

	/* Arithmetic operators */
	PLUS, MINUS, STAR, SLASH, EQUAL, NOT_EQUAL,
	GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

	/* Punctuation, Parentheses and Others */
	SEMICOLON, COMMA, DOT, EQUALEQUAL,
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	END_STM,  UNKNOWN, ERROR,

	/* End of file */
	EOF_TOKEN
};

/* token struct */
struct Token {

	TokenType 	type;
	std::string lexeme;
	int			line;

	Token(TokenType type, std::string lexeme, int line) : type(type), lexeme(lexeme), line(line) {};
};

/* convert type to string */
std::string tokenTypeToString(TokenType type);

#endif