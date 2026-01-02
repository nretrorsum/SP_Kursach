/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V07 programming language
 * Variant: Voitovych Oleksandr Viktorovych
 *
 * File: parser.h
 * Description: Syntax analyzer header file
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "symbol_table.h"
#include <vector>
#include <string>
#include <memory>

// AST (Abstract Syntax Tree) nodes
struct ASTNode {
    virtual ~ASTNode() = default;
};

// Expression
struct Expression : ASTNode {
    virtual ~Expression() = default;
};

// Number
struct NumberExpr : Expression {
    int value;
    NumberExpr(int v) : value(v) {}
};

// Identifier
struct IdentifierExpr : Expression {
    std::string name;
    IdentifierExpr(const std::string& n) : name(n) {}
};

// Unary expression
struct UnaryExpr : Expression {
    std::string op;
    std::unique_ptr<Expression> operand;
    UnaryExpr(const std::string& o, std::unique_ptr<Expression> e)
        : op(o), operand(std::move(e)) {}
};

// Binary expression
struct BinaryExpr : Expression {
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    BinaryExpr(const std::string& o, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

// Statement
struct Statement : ASTNode {
    virtual ~Statement() = default;
};

// Assignment
struct AssignmentStmt : Statement {
    std::string variable;
    std::unique_ptr<Expression> expression;
    AssignmentStmt(const std::string& v, std::unique_ptr<Expression> e)
        : variable(v), expression(std::move(e)) {}
};

// Input (Get)
struct GetStmt : Statement {
    std::string variable;
    GetStmt(const std::string& v) : variable(v) {}
};

// Output (Put)
struct PutStmt : Statement {
    std::unique_ptr<Expression> expression;
    PutStmt(std::unique_ptr<Expression> e) : expression(std::move(e)) {}
};

// Label
struct LabelStmt : Statement {
    std::string name;
    LabelStmt(const std::string& n) : name(n) {}
};

// Goto
struct GotoStmt : Statement {
    std::string label;
    GotoStmt(const std::string& l) : label(l) {}
};

// If-Goto
struct IfGotoStmt : Statement {
    std::unique_ptr<Expression> condition;
    std::string label;
    IfGotoStmt(std::unique_ptr<Expression> c, const std::string& l)
        : condition(std::move(c)), label(l) {}
};

// For-To-Next
struct ForStmt : Statement {
    std::string variable;
    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> end;
    std::vector<std::unique_ptr<Statement>> body;
    ForStmt(const std::string& v, std::unique_ptr<Expression> s, std::unique_ptr<Expression> e)
        : variable(v), start(std::move(s)), end(std::move(e)) {}
};

// Block statement (Begin-End)
struct BlockStmt : Statement {
    std::vector<std::unique_ptr<Statement>> statements;
};

// Program
struct Program : ASTNode {
    std::string name;
    std::vector<std::string> variables;
    std::vector<std::unique_ptr<Statement>> statements;
};

// Syntax analyzer
class Parser {
public:
    Parser(const std::vector<Token>& tokens);

    // Parse program
    std::unique_ptr<Program> parse();

    // Check if there are errors
    bool hasErrors() const { return !errors.empty(); }

    // Get error list
    const std::vector<std::string>& getErrors() const { return errors; }

    // Get symbol table
    SymbolTable& getSymbolTable() { return symbolTable; }

private:
    std::vector<Token> tokens;
    size_t current;
    std::vector<std::string> errors;
    SymbolTable symbolTable;

    // Helper methods
    Token peek() const;
    Token previous() const;
    Token advance();
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    void addError(const std::string& message);
    void synchronize();

    // Program structure parsing
    void parseProgram(Program& program);
    void parseVarDeclaration(Program& program);
    void parseStatements(std::vector<std::unique_ptr<Statement>>& statements);

    // Statement parsing
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseAssignment();
    std::unique_ptr<Statement> parseGet();
    std::unique_ptr<Statement> parsePut();
    std::unique_ptr<Statement> parseIf();
    std::unique_ptr<Statement> parseGoto();
    std::unique_ptr<Statement> parseFor();
    std::unique_ptr<Statement> parseBlock();
    std::unique_ptr<Statement> parseLabel();

    // Expression parsing
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseOr();
    std::unique_ptr<Expression> parseAnd();
    std::unique_ptr<Expression> parseEquality();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseAddition();
    std::unique_ptr<Expression> parseMultiplication();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parsePrimary();
};

#endif // PARSER_H
