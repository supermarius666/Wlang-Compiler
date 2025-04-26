#ifndef WLANG_LEXER_HPP
#define WLANG_LEXER_HPP

#include "Token.hpp"
#include "Utils.hpp"
#include <vector>
#include <unordered_map>
#include <iostream>

class Lexer {

	public:
		Lexer(const std::string& source) : source(source) {};
		~Lexer();
		std::vector<Token> tokenize();
	private:
		const std::string&	source; /* raw source code as a simple string */
		int start;	/* first character in the lexeme being scanned */
		int current; /* character currently being considered */
		int line = 1; /* tracks what source line current is on */

		/* helper functions */

		/* tells if all the characters are consumed */
		bool isAtEnd();

		/* consumes the next character in the source file
		 and returns it.*/
        char advance();
        
		/* lookahead: only looks at the current unconsumed character */
		char peek();


        char peekNext();
		
		/*recognize lexemes in two stages, 
		consume the current character if it’s what I'm looking for
		*/
		bool match(char expected);  
        
		/* skips white space and new line */
		void skipWhiteSpace();


        bool isAlpha(char c); 
		
		/* prints an error message */
		void reportError(const std::string& message) const;

		/* literals */

		Token identifier();
		Token number();
		Token string();

   /* Build token */
   Token makeToken(TokenType type, const std::string& lexeme);
};

#endif


  