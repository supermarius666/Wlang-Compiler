#include "../../include/CodeGen/common.h"
#include "../../include/Lexer/scanner.h"
#include <stdio.h>
#include <string.h>

/* variabile globale scanner (lexer)*/
Scanner	scanner;

void	initScanner(const char *source)
{
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}