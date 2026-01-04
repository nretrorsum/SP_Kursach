/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
 *
 * File: parser.h
 * Description: Syntax analyzer header file with AST definitions
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "symbol_table.h"
#include <vector>
#include <memory>
#include <string>

// Forward declarations
struct Expression;
struct Statement;

// Expression types
enum class ExprType {
    NUMBER,
    IDENTIFIER,
    BINARY,
    UNARY
};

// Base expression class
struct Expression {
    ExprType type;
    int line;

    Expression(ExprType t, int l) : type(t), line(l) {}
    virtual ~Expression() = default;
};

// Number literal
struct NumberExpr : Expression {
    int value;

    NumberExpr(int v, int l) : Expression(ExprType::NUMBER, l), value(v) {}
};

// Identifier (variable reference)
struct IdentifierExpr : Expression {
    std::string name;

    IdentifierExpr(const std::string& n, int l) : Expression(ExprType::IDENTIFIER, l), name(n) {}
};

// Binary expression
struct BinaryExpr : Expression {
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

    BinaryExpr(const std::string& o, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, int line)
        : Expression(ExprType::BINARY, line), op(o), left(std::move(l)), right(std::move(r)) {}
};

// Unary expression
struct UnaryExpr : Expression {
    std::string op;
    std::unique_ptr<Expression> operand;

    UnaryExpr(const std::string& o, std::unique_ptr<Expression> e, int l)
        : Expression(ExprType::UNARY, l), op(o), operand(std::move(e)) {}
};

// Statement types
enum class StmtType {
    ASSIGNMENT,
    GET,
    PUT,
    IF,
    WHILE,
    BLOCK
};

// Base statement class
struct Statement {
    StmtType type;
    int line;

    Statement(StmtType t, int l) : type(t), line(l) {}
    virtual ~Statement() = default;
};

// Assignment statement: _VAR :: expression
struct AssignmentStmt : Statement {
    std::string variable;
    std::unique_ptr<Expression> expression;

    AssignmentStmt(const std::string& var, std::unique_ptr<Expression> expr, int l)
        : Statement(StmtType::ASSIGNMENT, l), variable(var), expression(std::move(expr)) {}
};

// Get statement: get(_VAR)
struct GetStmt : Statement {
    std::string variable;

    GetStmt(const std::string& var, int l) : Statement(StmtType::GET, l), variable(var) {}
};

// Put statement: put(expression)
struct PutStmt : Statement {
    std::unique_ptr<Expression> expression;

    PutStmt(std::unique_ptr<Expression> expr, int l)
        : Statement(StmtType::PUT, l), expression(std::move(expr)) {}
};

// If statement (Rust style): if condition startblok ... endblok [else startblok ... endblok]
struct IfStmt : Statement {
    std::unique_ptr<Expression> condition;
    std::vector<std::unique_ptr<Statement>> thenBranch;
    std::vector<std::unique_ptr<Statement>> elseBranch;

    IfStmt(std::unique_ptr<Expression> cond, int l)
        : Statement(StmtType::IF, l), condition(std::move(cond)) {}
};

// While statement (Rust style): while condition startblok ... endblok
struct WhileStmt : Statement {
    std::unique_ptr<Expression> condition;
    std::vector<std::unique_ptr<Statement>> body;

    WhileStmt(std::unique_ptr<Expression> cond, int l)
        : Statement(StmtType::WHILE, l), condition(std::move(cond)) {}
};

// Block statement: startblok ... endblok
struct BlockStmt : Statement {
    std::vector<std::unique_ptr<Statement>> statements;

    BlockStmt(int l) : Statement(StmtType::BLOCK, l) {}
};

// Program structure
struct Program {
    std::vector<std::string> variables;
    std::vector<std::unique_ptr<Statement>> statements;
};

// Parser class
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    // Main parsing method
    std::unique_ptr<Program> parse();

    // Error handling
    bool hasErrors() const;
    const std::vector<std::string>& getErrors() const;

    // Symbol table access
    SymbolTable& getSymbolTable();

private:
    std::vector<Token> tokens;
    size_t current;
    std::vector<std::string> errors;
    SymbolTable symbolTable;

    // Helper methods
    Token currentToken() const;
    Token peekToken() const;
    void advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    void expect(TokenType type, const std::string& message);

    // Parsing methods
    void parseProgram(Program& program);
    void parseVariableDeclaration(Program& program);
    void parseStatements(std::vector<std::unique_ptr<Statement>>& statements);
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseAssignment();
    std::unique_ptr<Statement> parseGet();
    std::unique_ptr<Statement> parsePut();
    std::unique_ptr<Statement> parseIf();
    std::unique_ptr<Statement> parseWhile();
    std::unique_ptr<Statement> parseBlock();

    // Expression parsing (precedence climbing)
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseLogicalOr();
    std::unique_ptr<Expression> parseLogicalAnd();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseAdditive();
    std::unique_ptr<Expression> parseMultiplicative();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parsePrimary();

    // Error reporting
    void addError(const std::string& message);
};

#endif // PARSER_H
