#ifndef WLANG_BYTE_H
# define WLANG_BYTE_H

# include "object.h"

/* funzione che serve a scrivere il bytecode su un file */
bool 		writeFunctionToFile(ObjFunction *fun, const char *filename);

/* funzione che serve a leggere il bytecode da un file binario .wlb */
ObjFunction	*readFunctionFromFile(const char *filename);

#endif