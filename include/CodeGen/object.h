#ifndef WLANG_OBJECT_H
# define WLANG_OBJECT_H

# include "common.h"
# include "chunk.h"
# include "value.h"

/* semplice macro per prendere il tipo di un oggetto */
# define OBJ_TYPE(value)	(AS_OBJ(value)->type)

/* macro che verifica se ho un object string*/
# define IS_STRING(value)	isObjType(value, OBJ_STRING)

# define IS_FUNCTION(value)	isObjType(value, OBJ_FUNCTION)

/* ritorna il puntatore al oggetto stringa */
# define AS_STRING(value) 	((ObjString *)AS_OBJ(value))

/* ritorna l'array di caratteri, quinidi la stringa */
# define AS_CSTRING(value)	(((ObjString *)AS_OBJ(value))->chars)

/* faccio il cast al Objfunction pointer */
# define AS_FUNCTION(value)	((ObjFunction *)AS_OBJ(value))

/* enum per i vari tipi di oggetto */
typedef enum
{
	OBJ_STRING,		/* oggetto di tipo stringa */
	OBJ_FUNCTION,	/* oggetto di tipo funzione */
}	ObjType;

/* struct oggetto */
struct Obj
{
	ObjType		type;	/* tipo di oggetto: stringa, funzione, ... */
	struct Obj	*next;	/* puntatore al prossimo nodo nella lista di oggetti --> da cambiare in futuro */
};

/* oggetto funzione che ha un oggetto associato, numero di parametri, un chunk di codice associato e un nome */
typedef struct 
{
	Obj			obj;
	int 		arity;
	Chunk		chunk;
	ObjString	*name;
}	ObjFunction;

/* struct oggetto stringa */
struct ObjString
{
	Obj			obj;		/* l'ogetto associato --> cosi derivo dalla classe padre Obj (OOP IN C)*/
	int			lenght;		/* lunghezza stringa */
	char		*chars;		/* contenuto stringa */
	uint32_t	hash;		/* hash della stringa */
};	

/* funzione che crea una nuova funzione */
ObjFunction	*newFunction();

/* funzione che alloca e restituisce una stringa */
ObjString	*takeString(char *chars, int lenght);

/* funzione che fa copy dei caratteri della stringa come lexema in un array di chars*/
ObjString	*copyString(const char *chars, int lenght);

/* funzione di stampa per gli oggetti(stringhe) */
void		printObject(Value value);

/* funzione che verifica che il valore è un oggetto e che è del tipo specificato */
static inline bool isObjType(Value value, ObjType type)
{
	return (IS_OBJ(value) && AS_OBJ(value)->type == type);
}

#endif