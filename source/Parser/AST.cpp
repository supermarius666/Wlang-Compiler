#include "../../include/Parser/AST.hpp"

bool LiteralExpr::isString() {
	return value.type == TokenType::STRING;
}

bool LiteralExpr::isNumber() {
	return value.type == TokenType::NUMBER;
}

bool LiteralExpr::isBool() {
	return (value.type == TokenType::TRUE || value.type == TokenType::FALSE);
}

