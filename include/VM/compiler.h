#ifndef WLANG_COMPILER_H
# define WLANG_COMPILER_H

# include "../CodeGen/object.h"
# include "vm.h"


/* compila il codice in un chunk */
ObjFunction	*compile(const char *source);



#endif