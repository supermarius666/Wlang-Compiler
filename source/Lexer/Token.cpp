#include "../../include/Lexer/Token.hpp"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IDENTIFIER:         return "IDENTIFIER";
        case TokenType::MAIN:               return "MAIN";
        case TokenType::ERROR:              return "ERROR";
        case TokenType::FUN:           		return "FUN";
        case TokenType::COMMA:              return "COMMA";
        case TokenType::DOT:                return "DOT";
        case TokenType::VERO:               return "TRUE";
        case TokenType::FALSO:              return "FALSE";
        case TokenType::NUMBER:             return "NUMBER";
        case TokenType::STRING:             return "STRING";
        case TokenType::ALTRIMENTI:         return "ALTRIMENTI";
        case TokenType::STAMPA:             return "STAMPA";
        case TokenType::SIA:                return "SIA";
        case TokenType::SE:                 return "SE";
        case TokenType::PLUS:               return "PLUS";
        case TokenType::MINUS:              return "MINUS";
        case TokenType::STAR:               return "STAR";
        case TokenType::SLASH:              return "SLASH";
        case TokenType::EQUAL:              return "EQUAL";
        case TokenType::EQUALEQUAL:         return "EQUALEQUAL";
        case TokenType::NOT_EQUAL:          return "NOT_EQUAL";
        case TokenType::GREATER:            return "GREATER";
        case TokenType::GREATER_EQUAL:      return "GREATER_EQUAL";
        case TokenType::LESS:               return "LESS";
        case TokenType::LESS_EQUAL:         return "LESS_EQUAL";
        case TokenType::E:                  return "AND";
        case TokenType::O:                  return "OR";
        case TokenType::NON:                return "NOT";
        case TokenType::LEFT_PAREN:         return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:        return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:         return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:        return "RIGHT_BRACE";
        case TokenType::SEMICOLON:          return "SEMICOLON";
        case TokenType::EOF_TOKEN:          return "EOF_TOKEN";
        case TokenType::END_STM:            return "END_STM";
        case TokenType::UNKNOWN:            return "UNKNOWN";
        default:                            return "UNKNOWN";
    }
}