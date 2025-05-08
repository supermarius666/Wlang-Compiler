#include "../../include/CodeGen/common.h"
#include "../../include/VM/compiler.h"
#include "../../include/Lexer/scanner.h"
#include <stdio.h>


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
		printf("%2d '%.*s'\n", token.type, token.lenght, token.start);

		if (token.type == EOF_TOKEN) break;
	}
}