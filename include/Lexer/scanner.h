#ifndef WLANG_SCANNER_H
# define WLANG_SCANNER_H

/* scanner che fa il lexing del codice sorgente  */
typedef struct {
	const char	*start;
	const char	*current;
	int			line;
}	Scanner;

/* inizializza lo scanner */
void	initScanner(const char *source);

#endif