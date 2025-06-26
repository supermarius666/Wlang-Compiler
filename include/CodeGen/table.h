#ifndef WLANG_TABLE_H
# define WLANG_TABLE_H

# include "common.h"
# include "value.h"

# define TABLE_MAX_LOAD 0.75  /* 75% */ 

/* entry nella hash table */
typedef struct 
{
	ObjString		*key;
	Value			value;
}	Entry;


/* hash table struct */
typedef struct 
{
	int		count;		/* numero di key nella hash table */
	int		capacity;	/* capacità massima hash table */
	Entry	*entries;	
}	Table;


void		initTable(Table *table);
void		freeTable(Table *table);
bool		tableGet(Table *table, ObjString *key, Value *value);
bool		tableSet(Table *table, ObjString *key, Value value);
bool		tableDelete(Table *table, ObjString *key);
void		tableAddAll(Table *from, Table *to);
ObjString	*tableFindString(Table *table, const char *chars, int lenght, uint32_t hash);
#endif