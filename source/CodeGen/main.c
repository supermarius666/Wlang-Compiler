#include "../../include/CodeGen/common.h"
#include "../../include/CodeGen/chunk.h"
#include "../../include/CodeGen/debug.h"
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
		printf("============================= Wlang Compiler =============================\n");
		printf("         Empowering your code with precision and power.\t\t\n");
		printf("        Official Version: 1.0 - Build 2025-04-23\t\t\n");
		printf("==========================================================================\n");
		printf("Usage: wlang -name executable_name <files.wl>\n");
		printf("This compiler is licensed under the Wlang Open License (WOL).\n");
		printf("Redistribution and usage in source and binary forms, with or without modification,\n");
		printf("are permitted provided that the following conditions are met:\n");
		printf("1. Original copyright notice and disclaimers must be retained.\n");
		printf("2. This software may not be used for evil purposes.\n");
		printf("==========================================================================\n");
		printf("Copyright (C) 2025 Wlang Project. All rights reserved.\n");
		printf("==========================================================================\n");
		freeVm();									// libera Vm
		return (0);

	}
	else if (argc != 4 || strcmp(argv[1], "-name") != 0) {
		fprintf(stderr, "Uso: %s -name nome_eseguibile file.wl\n", argv[0]);
        return 1;
	}
	else
	{
		/* parte che creerà il file binario .wlb*/
		output_file = argv[2];
		input_path = argv[3];
		runFile(argv[3]);
		char filename[256];
		snprintf(filename, sizeof(filename), "%s.wlb", output_file);
		FILE* out = fopen(filename, "wb");
		if (!out) {
			perror("Errore creazione eseguibile");
			return 1;
		}
	}
		
	return (0);
}