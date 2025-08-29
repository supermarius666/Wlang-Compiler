#include "../../include/CodeGen/common.h"
#include "../../include/CodeGen/chunk.h"
#include "../../include/CodeGen/debug.h"
#include "../../include/CodeGen/byte.h"
#include "../../include/VM/compiler.h"
#include "../../include/VM/vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* output_file = NULL;
const char* input_path = NULL;

static char *readFile(const char *path)
{
	FILE *file = fopen(path, "rb");
	if (!file)
	{
		fprintf(stderr, "Could not open the file '%s' \n", path);
		exit(74);
	}

	/* calcolo la grandezza del file */
	fseek(file, 0L, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	/* leggo tutto il file in un buffer */
	char *buffer = (char *)malloc(fileSize + 1);
	if(!buffer)
	{
		fprintf(stderr, "Not enough memory to read '%s' \n", path);
		exit(74);
	}
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file); 
	if (bytesRead < fileSize)
	{
		fprintf(stderr, "Could not read the file '%s' \n", path);
		exit(74);
	}
	buffer[bytesRead] = '\0';

	/* chiudo il file e ritorno il buffer */
	fclose(file);
	return (buffer);
}

static void runFile(const char *path)
{
	char *source = readFile(path);
	InterpretResult result = interpret(source);
	free(source);

	if (result == INTERPRET_COMPILE_ERROR) exit(65);
	if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, char **argv) 
{
	initVm();										// inizializza Vm 
	if (argc == 1)
	{
		printf("============================= Compilatore Wlang =============================\n");
		printf("          Potenzia il tuo codice con precisione e potenza.\t\t\n");
		printf("           Versione ufficiale: 1.0 - Build 2025-04-23\t\t\n");
		printf("=============================================================================\n");
		printf("Questo compilatore è rilasciato sotto la Wlang Open License (WOL).\n");
		printf("La redistribuzione e l'utilizzo, in forma sorgente o binaria, con o senza modifiche,\n");
		printf("sono consentiti a condizione che siano rispettati i seguenti punti:\n");
		printf("1. Le note di copyright originali e le clausole di esclusione di responsabilità\n");
		printf("   devono essere mantenute.\n");
		printf("2. Questo software non può essere utilizzato per scopi maligni.\n");
		printf("=============================================================================\n");
		printf("Copyright (C) 2025 Progetto Wlang. Tutti i diritti riservati.\n");
		printf("=============================================================================\n");
		freeVm();  // libera Vm
		return (0);
	}


	if (argc == 2 && (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "--help") == 0))
	{
		printf("Uso: wlang [opzioni] <file>\n\n");
		printf("Opzioni disponibili:\n");
		printf("  wlang file.wl             				Compila ed esegue subito il file sorgente.\n");
		printf("  wlang -name <nome> file.wl 				Compila e salva 'nome.wlb' nella cartella corrente.\n");
		printf("  wlang -name <nome> file.wl -path <dir> 		Compila e salva 'nome.wlb' dentro 'dir'.\n");
		printf("  wlang -b file.wlb         				Carica ed esegue il bytecode da file.wlb.\n");
		printf("  wlang -help               				Mostra questo messaggio di aiuto.\n");
		freeVm();
		return (0);
	}

    if (argc == 2 && argv[1][0] != '-') {
        runFile(argv[1]);
        freeVm();
        return 0;
    }

	// wlang -name name file.wl  -> compila e salva name.wlb (non esegue)
	if ((argc == 4 && strcmp(argv[1], "-name") == 0) || (argc == 6 && strcmp(argv[1], "-name") == 0 && strcmp(argv[4], "-path") == 0)) 
	{
		output_file = argv[2];   // nome base
		input_path = argv[3];    // file sorgente

		char *source = readFile(input_path);
		ObjFunction *fn = compile(source);
		free(source);
		if (!fn) 
		{ 
			freeVm(); 
			return (65); 
		}

		char filename[512];
		if (argc == 6) {
			// con -path: salvo <path>
			snprintf(filename, sizeof(filename), "%s/%s.wlb", argv[5], output_file);
		} else {
			// senza -path: salvo nella cartella corrente
			snprintf(filename, sizeof(filename), "%s.wlb", output_file);
		}

		if (!writeFunctionToFile(fn, filename)) {
			fprintf(stderr, "Errore scrittura bytecode in '%s'.\n", filename);
			freeVm(); 
			return (74);
		}

		printf("Bytecode salvato in: %s\n", filename);
		freeVm(); 	
		return (0);
	}

	// wlang -b file.wlb  -> carica bytecode ed esegue
	if (argc == 3 && strcmp(argv[1], "-b") == 0) 
	{
		const char *bytecodePath = argv[2];
		ObjFunction *fn = readFunctionFromFile(bytecodePath);
		if (!fn) 
		{ 
			fprintf(stderr, "Impossibile leggere bytecode da '%s'.\n", bytecodePath); 
			freeVm(); 
			return (74); 
		}
		InterpretResult r = runCompiled(fn); 
		freeVm(); 
		return ((r == INTERPRET_OK) ? 0 : 70);
	}

	return (1);
}