#ifndef WLANG_VISITOR_HPP
#define WLANG_VISITOR_HPP

#include <variant>
#include "AST.hpp"

/* for different outputs */
using Value = std::variant<int, std::string, bool>;

/* Visitor class --> abstract class */
class Visitor {
	public:
		virtual Value visitLiteralExpr(LiteralExpr& expr) = 0;
		virtual Value visitBinaryExpr(BinaryExpr& expr) = 0;
		virtual Value visitUnaryExpr(UnaryExpr& expr) = 0;
		virtual Value visitGroupingExpr(GroupingExpr& expr) = 0;
};

#endif