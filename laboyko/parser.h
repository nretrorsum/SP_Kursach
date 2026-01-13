#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <memory>
#include "token.h"
#include "symbol_table.h"

// Forward declarations
struct Expression;
struct Statement;

// Expression types
struct Expression {
    virtual ~Expression() = default;
    int line = 0;
};

struct NumberExpr : Expression {
    int value;
    NumberExpr(int v, int l) : value(v) { line = l; }
};

struct IdentifierExpr : Expression {
    std::string name;
    IdentifierExpr(const std::string& n, int l) : name(n) { line = l; }
};

struct BinaryExpr : Expression {
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    BinaryExpr(const std::string& o, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, int ln)
        : op(o), left(std::move(l)), right(std::move(r)) { line = ln; }
};

struct UnaryExpr : Expression {
    std::string op;
    std::unique_ptr<Expression> operand;
    UnaryExpr(const std::string& o, std::unique_ptr<Expression> e, int l)
        : op(o), operand(std::move(e)) { line = l; }
};

// Statement types
struct Statement {
    virtual ~Statement() = default;
    int line = 0;
};

struct AssignStmt : Statement {
    std::string variable;
    std::unique_ptr<Expression> value;
    AssignStmt(const std::string& var, std::unique_ptr<Expression> val, int l)
        : variable(var), value(std::move(val)) { line = l; }
};

struct GetStmt : Statement {
    std::string variable;
    GetStmt(const std::string& var, int l) : variable(var) { line = l; }
};

struct PutStmt : Statement {
    std::unique_ptr<Expression> value;
    PutStmt(std::unique_ptr<Expression> val, int l) : value(std::move(val)) { line = l; }
};

struct LabelStmt : Statement {
    std::string name;
    LabelStmt(const std::string& n, int l) : name(n) { line = l; }
};

struct GotoStmt : Statement {
    std::string label;
    GotoStmt(const std::string& lbl, int l) : label(lbl) { line = l; }
};

struct IfGotoStmt : Statement {
    std::unique_ptr<Expression> condition;
    std::string label;
    IfGotoStmt(std::unique_ptr<Expression> cond, const std::string& lbl, int l)
        : condition(std::move(cond)), label(lbl) { line = l; }
};

struct ForStmt : Statement {
    std::string variable;
    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> end;
    std::vector<std::unique_ptr<Statement>> body;
    ForStmt(const std::string& var, std::unique_ptr<Expression> s, std::unique_ptr<Expression> e, int l)
        : variable(var), start(std::move(s)), end(std::move(e)) { line = l; }
};

// Program structure
struct Program {
    std::string name;
    std::vector<std::string> variables;
    std::vector<std::unique_ptr<Statement>> statements;
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos;
    std::vector<std::string> errors;
    SymbolTable& symbolTable;

    Token& currentToken();
    Token& peekToken(int offset = 1);
    void advance();
    bool match(TokenType type);
    bool check(TokenType type);
    void expect(TokenType type, const std::string& message);

    void parseVariableDeclarations(Program& program);
    void parseStatements(std::vector<std::unique_ptr<Statement>>& statements);
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseAssignment();
    std::unique_ptr<Statement> parseGet();
    std::unique_ptr<Statement> parsePut();
    std::unique_ptr<Statement> parseIfGoto();
    std::unique_ptr<Statement> parseGoto();
    std::unique_ptr<Statement> parseFor();
    std::unique_ptr<Statement> parseLabel();

    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseLogicalOr();
    std::unique_ptr<Expression> parseLogicalAnd();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseAddSub();
    std::unique_ptr<Expression> parseMulDiv();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parsePrimary();

public:
    Parser(const std::vector<Token>& tokens, SymbolTable& symTable);
    std::unique_ptr<Program> parse();
    const std::vector<std::string>& getErrors() const { return errors; }
};

#endif // PARSER_H
