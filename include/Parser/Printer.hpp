#pragma once

#include "Visitor.hpp"
#include "AST.hpp"
#include <iostream>


class ASTPrinter : public Visitor {
	public:
		int indent = 0;
		void printExpr(Expr* expr, int indent) {
			if (expr) {
				expr->accept(*this);  
			}
		}
	
	
		// Implementazione dei metodi del Visitor per ogni tipo di espressione
		Value visitLiteralExpr(LiteralExpr& expr) override {
			std::cout << std::string(indent, ' ') << "Literal: " << expr.value.lexeme << "\n";
			return {};
		}
	
		Value visitBinaryExpr(BinaryExpr& expr) override {
			std::cout << std::string(indent, ' ') << "BinaryExpr: " << expr.op.lexeme << "\n";
			printExpr(expr.left.get(), indent + 2);
			printExpr(expr.right.get(), indent + 2);
			return {};
		}

		Value visitUnaryExpr(UnaryExpr& expr) override {
			std::cout << std::string(indent, ' ') << "BinaryExpr: " << expr.op.lexeme << "\n";
			printExpr(expr.expr.get(), indent + 2);
			printExpr(expr.expr.get(), indent + 2);
			return {};
		}
	
		Value visitGroupingExpr(GroupingExpr& expr) override {
			std::cout << std::string(indent, ' ') << "Grouping:\n";
			printExpr(expr.expr.get(), indent + 2);
			return {};
		}
	
	};
	