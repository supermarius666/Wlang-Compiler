#ifndef WLANG_COMPILER_H
# define WLANG_COMPILER_H

# include "vm.h"

/* compila il codice in un chunk */
bool	compile(const char *source, Chunk *chunk);

#endif