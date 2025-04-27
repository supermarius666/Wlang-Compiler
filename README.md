# Wlang-Compiler
For my Operating Systems course at university, I developed a simple compiler for a small language I designed for fun. The project also includes a virtual machine simulation, as the compiler generates bytecode to be executed on it.


## Lexer
- [ ] Finish documentation
- [x] Error handling
- [ ] Testing
- [x] Keywords

Lexer’s grammar  -> the alphabet consists of individual characters and the strings are the valid lexemes

## Parser
- [ ] Finish documentation
- [ ] Error handling
- [ ] Testing
- [ ] Grammar Rappresentation (Backus-Naur form)

Syntactic grammar(Parser's grammar) ->  each “letter” in the alphabet is an entire token and a “string” is a sequence of tokens, an entire expression.

### Grammar
Set of rules. Rules are called productions because they produce strings in the grammar.You can use them to generate strings that are in the grammar.  Strings created this way are called derivations because each is “derived” from the rules of the grammar.

Each production in a context-free grammar has a head—its name—and a body 
which describes what it generates. In its pure form, the body is simply a list of
symbols. Symbols come in two delectable flavors:
A terminal is a letter from the grammar’s alphabet. You can think of it like a
literal value. In the syntactic grammar we’re defining, the terminals are
individual lexemes—tokens coming from the scanner like if or 1234.
These are called “terminals”, in the sense of an “end point” because they don’t lead to any further rule.
A nonterminal is a named reference to another rule in the grammar. It
means “play that rule and insert whatever it produces here”. In this way, the grammar composes.


### The Visitor pattern
The Visitor Pattern is a design pattern that separates algorithms from the object structure on which they operate. It allows adding new operations to a set of classes without modifying them, by defining a visitor class that implements an operation for each type of object in the structure.

Abstract class
	--> Interpreter
	--> Printer
	--> Opimizer 

- [ ] Problems with expr, the Printer and pointers