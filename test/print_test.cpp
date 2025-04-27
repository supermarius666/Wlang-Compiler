#include "../include/Lexer/Lexer.hpp"
#include "../include/Lexer/Utils.hpp"
#include "../include/Parser/Printer.hpp"
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
		
		std::unique_ptr<Expr> expr = std::make_unique<BinaryExpr>(
			std::make_unique<LiteralExpr>(Token(TokenType::NUMBER, "3", 1)),
			std::make_unique<LiteralExpr>(Token(TokenType::NUMBER, "4", 1)),
			Token(TokenType::PLUS, "+", 1)
		);

		ASTPrinter printer;

		printer.printExpr(expr.get(), 0);
	
    }

    return 0;

}
