#include <stdlib.h>
#include <string.h>

#include "../../include/CodeGen/memory.h"
#include "../../include/CodeGen/object.h"
#include "../../include/CodeGen/table.h"
#include "../../include/CodeGen/value.h"

/* inizializza la hash table */
void	initTable(Table *table)
{
	table->count = 0;
	table->capacity = 0;
	table->entries = NULL;
}

/* free hash table */
void	freeTable(Table *table)
{
	FREE_ARRAY(Entry, table->entries, table->capacity);
	initTable(table);
}


/*	questa funzione prende la chiave e ritorna un puntatore al bucket in cui si trova 
	risolvo le collissioni con linear probing 
*/
static Entry *findEntry(Entry *entries, int capacity, ObjString *key)
{
	uint32_t index = key->hash % capacity;
	Entry *tombstone = NULL;
	for (;;)
	{
		Entry *entry = &entries[index];

		if (entry->key == NULL)
		{
			if (IS_NIL(entry->value))
				return (tombstone != NULL ? tombstone : entry);
			else
				if (tombstone == NULL) tombstone = entry;
		}
		else if (entry->key == key)
			return (entry);	
		index = (index + 1) % capacity;
	}
}

/* aumenta la dimensione della tabella hash in base al load factor */
static void adjustCapacity(Table *table, int capacity)
{
	Entry *entries = ALLOCATE(Entry, capacity);
	for (int i = 0; i < capacity; i++)
	{
		entries[i].key = NULL;
		entries[i].value = NIL_VAL;
	}

	// reinserico tutti gli elementi nella nuova tabella 
	table->count = 0;
	for (int i = 0; i < table->capacity; i++)
	{
		Entry *entry = &table->entries[i];
		if (entry->key == NULL) continue;

		Entry *dest = findEntry(entries, capacity, entry->key);
		dest->key = entry->key;
		dest->value = entry->value;
		table->count++;
	}

	FREE_ARRAY(Entry, table->entries, table->capacity);
	table->entries = entries;
	table->capacity = capacity;
}


/* 	funzione per inserire chiave valore
	se esiste già la coppia chiave/valore vine sovrascritta  
*/
bool	tableSet(Table *table, ObjString *key, Value value)
{

	if (table->count + 1 > table->capacity * TABLE_MAX_LOAD)
	{
		int capacity = GROW_CAPACITY(table->capacity);
		adjustCapacity(table, capacity);
	}

	Entry *entry = findEntry(table->entries, table->capacity, key);

	bool isNewKey = (entry->key == NULL);
	if (isNewKey && IS_NIL(entry->value)) table->count++;

	entry->key = key;
	entry->value = value;

	return (isNewKey);
}
/* 	funzione per eliminare chiave/valore 
	la chiave diventa NULL e il valore diventa true -->tombstone(perché non c'è più nulla, tipo la tomba)
*/
bool	tableDelete(Table *table, ObjString *key)
{
	if (table->count == 0) return (false);
	
	Entry *entry = findEntry(table->entries, table->capacity, key);
	if (entry->key == NULL) return(false);

	entry->key = NULL;
	entry->value = BOOL_VAL(true);

	return (true);
}

/* funzione per ottenere value a partire da una chiave */
bool	tableGet(Table *table, ObjString *key, Value *value)
{
	if (table->count == 0) return (false);

	Entry *entry = findEntry(table->entries, table->capacity, key);
	if(entry->key == NULL) return (false);

	*value = entry->value;
	return (true);
}

/* funzione per copiare una tabella di hash in un'altra */
void	tableAddAll(Table *from, Table *to)
{
	for (int i = 0; i < from->capacity; i++)
	{
		Entry *entry = &from->entries[i];
		if (entry->key != NULL)
			tableSet(to, entry->key, entry->value);
	}
}

ObjString *tableFindString(Table *table, const char *chars, int lenght, uint32_t hash)
{
	if (table->count == 0) return (NULL);

	uint32_t index = hash % table->capacity;

	for (;;)
	{
		Entry *entry = &table->entries[index];
		
		if (entry->key == NULL)
			if (IS_NIL(entry->value)) return (NULL);
		
		else if (	entry->key->lenght == lenght && 
					entry->key->hash == hash &&
					memcmp(entry->key->chars, chars, lenght) == 0
				)
			return (entry->key);
		
		index = (index + 1) % table->capacity;
	}
}
