#include "../../include/CodeGen/common.h"
#include "../../include/VM/compiler.h"
#include "../../include/Lexer/scanner.h"
#include <stdio.h>


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


void	compile(const char *source)
{
	initScanner(source);
	int line = -1;
	while (true)
	{
		Token token = scanToken();
		if (token.line != line)
		{
			printf("%4d ", token.line);
			line = token.line;
		}
		else
			printf("		| ");
		printf("%s '%.*s'\n", tokenTypeToString(token.type), token.lenght, token.start);

		if (token.type == EOF_TOKEN) break;
	}
}