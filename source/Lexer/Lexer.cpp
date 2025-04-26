#include "../../include/Lexer/Lexer.hpp"

Lexer::~Lexer() = default;

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;
	while (!isAtEnd()) {
		skipWhiteSpace();
		/* set the cursor ad the beginning of the next lexeme */
		start = current;
		
		/* id literals */
		if (isAlpha(peek())) tokens.push_back(identifier());

		/* number literals */
        else if (std::isdigit(peek())) tokens.push_back(number());

		/* string literals */
        else if (peek() == '"') tokens.push_back(string());
		else {
			char c = advance();
			switch (c)
			{
				case '"': string(); break;
				case '(': tokens.push_back(makeToken(TokenType::LEFT_PAREN, "(" )); break;
				case ')':  tokens.push_back(makeToken(TokenType::RIGHT_PAREN, ")" )); break;
				case '{':  tokens.push_back(makeToken(TokenType::LEFT_BRACE, "{" )); break;
				case '}':  tokens.push_back(makeToken(TokenType::RIGHT_BRACE, "}" )); break;
				case '+':  tokens.push_back(makeToken(TokenType::PLUS, "+" )); break;
				case '-':  tokens.push_back(makeToken(TokenType::MINUS, "-" )); break;
				case '*':  tokens.push_back(makeToken(TokenType::STAR, "*" )); break;
				case '/': 
					if (match('/')) {
						/* can add a single line of comment with //  */
						while ((peek() != '\n' ) && !isAtEnd()) advance();    /* don't add a token because we ignore comments */
					}else if (match('*')) {
						/* multiple line comment (C like) */
						while (!isAtEnd()) {
							if (peek() == '*' && peekNext() == '/') {
								advance(); // consume '*'
								advance(); // consume '/'
								break;
							} else {
								if (peek() == '\n') line++; 
								advance();
							}
						}
					} else { // for division 
						tokens.push_back(makeToken(TokenType::SLASH, "/" ));
					}
					break;
				case ';':  tokens.push_back(makeToken(TokenType::SEMICOLON, ";" )); break;
				case ',':  tokens.push_back(makeToken(TokenType::COMMA, "," )); break;
				case '!':
					if (match('!')) tokens.push_back(makeToken(TokenType::END_STM, "END_STM"));
					else if (match('=')) tokens.push_back(makeToken(TokenType::NOT_EQUAL, "!="));
					else { 
						reportError("Unexpected character.");
						// tokens.push_back(makeToken(TokenType::UNKNOWN, std::string(1, c)));
					}
					break;
				case '=':
					if (match('=')) tokens.push_back(makeToken(TokenType::EQUALEQUAL, "=="));
					else tokens.push_back(makeToken(TokenType::EQUAL, "="));
					break;
				case '<':
					if (match('=')) tokens.push_back(makeToken(TokenType::LESS_EQUAL, "<="));
					else tokens.push_back(makeToken(TokenType::LESS, "<"));
					break;
				case '>':
					if (match('=')) tokens.push_back(makeToken(TokenType::GREATER_EQUAL, ">="));
					else tokens.push_back(makeToken(TokenType::GREATER, ">"));
					break;
					default:
					if (isspace(c)) {
						if (c == '\n') line++;
					} else {
						reportError("Unexpected character.");
						// tokens.push_back(makeToken(TokenType::UNKNOWN, std::string(1, c)));
					}
					break;
			}
		}
	}

	tokens.push_back(makeToken(TokenType::EOF_TOKEN, "EOF_TOKEN"));
	return tokens;
}

bool Lexer::isAtEnd() {
	return current >= source.length();
}

char Lexer::advance() {
	return source[current++];
}

char Lexer::peek() {
	if (isAtEnd()) return '\0';
	return source[current];
}

char Lexer::peekNext() {
	return (current + 1 < source.length()) ? source[current + 1] : '\0';
}

bool Lexer::match(char expected) {
	if (isAtEnd()) return false;
	if (source[current] != expected) return false;

	current++;
	return true;
}

void Lexer::skipWhiteSpace() {
	while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') 
            advance();
        else if (c == '\n')  {
            line++;
            advance();
        }
        else break;
    }
}

bool Lexer::isAlpha(char c) {
	return (std::isalpha(static_cast<unsigned char>(c)) || c == '_');
}

void Lexer::reportError(const std::string& message) const {
   
    // 1. Calculate the start of the current line
    int lineStart = current;
    while (lineStart > 0 && source[lineStart - 1] != '\n') {
        lineStart--;
    }

    // 2. Calculate the end of the current line
    int lineEnd = current;
    while (lineEnd < source.size() && source[lineEnd] != '\n') {
        lineEnd++;
    }

    // 3. Extract the current line
    std::string currentLine = source.substr(lineStart, lineEnd - lineStart);

    // 4. Calculate the column
    int column = current - lineStart - 1;
    if (column < 0) column = 0;

    // 5. Print the error message
    char errorChar = (current - 1 >= 0 && current - 1 < source.size()) ? source[current - 1] : '?';

    std::cerr << RED << "Error: " << RESET << BLUE << message << " '" << errorChar << "'\n\n" << RESET;
    std::cerr << " " << line << " | " << currentLine << "\n";
    std::cerr << "   | ";

    for (int i = 0; i < column; ++i) {
        if (i < currentLine.size() && currentLine[i] == '\t')
            std::cerr << '\t';
        else
            std::cerr << ' ';
    }

    std::cerr << RED << "^-- Here." << RESET << "\n";
}



/* keywords map */
static const std::unordered_map<std::string, TokenType> keywords = {
	{"main", TokenType::MAIN},
	{"sia", TokenType::SIA},
	{"stampa", TokenType::STAMPA},
	{"se", TokenType::SE},
	{"altrimenti", TokenType::ALTRIMENTI},
	{"fun", TokenType::FUN},
	{"and", TokenType::AND},
	{"or", TokenType::OR},
	{"not", TokenType::NOT},
};


Token Lexer::identifier() {
    while (std::isalnum(peek())) advance();
    std::string id = source.substr(start, current - start);

    auto it = keywords.find(id);
    if (it != keywords.end())
        return makeToken(it->second, id);

    return makeToken(TokenType::IDENTIFIER, id);
}

Token Lexer::number() {
    while (std::isdigit(peek())) advance();

	// Look for a fractional part.
	if (peek() == '.' && std::isdigit(peekNext())) {
		// Consume the "."
		advance();
		while (std::isdigit(peek())) advance();
	}

    std::string id = source.substr(start, current - start);
    return makeToken(TokenType::NUMBER, id);
}

Token Lexer::string() {
    start = current;         
    advance();              

    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
		reportError("Unterminated string.");
		return makeToken(TokenType::UNKNOWN, "???");
    }

    advance(); // consuma chiusura "

    std::string value = source.substr(start + 1, current - start - 2);
    return makeToken(TokenType::STRING, value);
}

Token Lexer::makeToken(TokenType type, const std::string& lexeme) {
    return {type, lexeme, line};
}