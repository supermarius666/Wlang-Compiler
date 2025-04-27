#pragma once

#ifndef WLANG_AST_HPP
#define WLANG_AST_HPP

#include "../Lexer/Token.hpp"
#include "Visitor.hpp"
#include <memory>

/* for different outputs */
using Value = std::variant<int, std::string, bool>;

/* general expression class --> AST nodes */
class Expr {
	public:
		~Expr() = default;
		virtual Value accept(class Visitor& visitor) = 0;
};

/* Binary expression */
class BinaryExpr : public Expr {
	public:
		std::unique_ptr<Expr> left;
		Token op;
		std::unique_ptr<Expr> right;
	
		BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
			: left(std::move(left)), op(op), right(std::move(right)) {}
	
		Value accept(Visitor& visitor) override {
			return visitor.visitBinaryExpr(*this);
		}
};


/* Unary expression */
class UnaryExpr : public Expr {
	public:
		Token op;
		std::unique_ptr<Expr> expr;
	
		UnaryExpr(Token op, std::unique_ptr<Expr> expr)
			: op(op), expr(std::move(expr)) {}
	
		Value accept(Visitor& visitor) override {
			return visitor.visitUnaryExpr(*this);
		}
};


/* Grouping expression */
class GroupingExpr : public Expr {
	public:
		std::unique_ptr<Expr> expr;
	
		GroupingExpr(std::unique_ptr<Expr> expr)
			: expr(std::move(expr)) {}
	
		Value accept(Visitor& visitor) override {
			return visitor.visitGroupingExpr(*this);
		}
};


/* Literal expression */
class LiteralExpr : public Expr {
	public:
		Token value;
	
		explicit LiteralExpr(Token value)
			: value(std::move(value)) {}
	
		Value accept(Visitor& visitor) override {
			return visitor.visitLiteralExpr(*this);
		}
	
		bool isString();
		bool isNumber();
		bool isBool();
};

#endif