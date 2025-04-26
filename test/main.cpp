#include "../include/Lexer/Lexer.hpp"
#include "../include/Lexer/Utils.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {

	if (argc < 2) {
        std::cout << "============================= Wlang Compiler =============================\n";
        std::cout << "         Empowering your code with precision and power.\t\t\n";
        std::cout << "        Official Version: 1.0 - Build 2025-04-23\t\t\n";
        std::cout << "==========================================================================\n";
        std::cout << "Usage: wlang <file.wl>\n";
        std::cout << "This compiler is licensed under the Wlang Open License (WOL).\n";
        std::cout << "Redistribution and usage in source and binary forms, with or without modification,\n";
        std::cout << "are permitted provided that the following conditions are met:\n";
        std::cout << "1. Original copyright notice and disclaimers must be retained.\n";
        std::cout << "2. This software may not be used for evil purposes.\n";
        std::cout << "==========================================================================\n";
        std::cout << "Copyright (C) 2025 Wlang Project. All rights reserved.\n";
        std::cout << "==========================================================================\n";
        return 0;
    }

    if (argc > 2) {
        std::cerr <<  "Invalid usage\n";  
        return 1;
    }

    if (argc == 2) {
		
		/* get the source code */
        std::ifstream inputFile(argv[1]);
        if (!inputFile.is_open()) {
            std::cerr << "Could not open file: " << argv[1] << std::endl;
            return 1;
        }

       /* all code in one place --> single string */
        std::string sourceCode(
            (std::istreambuf_iterator<char>(inputFile)),
            std::istreambuf_iterator<char>()
        );
        	 
        Lexer lexer(sourceCode);
        auto tokens = lexer.tokenize();

        for (Token t: tokens) {
            std::cout << BLUE << "Token: " << RESET << MAGENTA << t.lexeme << RESET << RED <<" (Type: " << tokenTypeToString(t.type) << ")\n" << RESET;
        }

    }

    return 0;

}