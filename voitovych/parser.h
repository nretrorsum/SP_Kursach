/**
 * Курсовий проект з Системного Програмування
 * Тема: Розробка транслятора з вхідної мови програмування V07
 * Варіант: Войтович Олександр Вікторович
 *
 * Файл: parser.h
 * Опис: Заголовний файл синтаксичного аналізатора
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "symbol_table.h"
#include <vector>
#include <string>
#include <memory>

// Вузли AST (Abstract Syntax Tree)
struct ASTNode {
    virtual ~ASTNode() = default;
};

// Вираз
struct Expression : ASTNode {
    virtual ~Expression() = default;
};

// Число
struct NumberExpr : Expression {
    int value;
    NumberExpr(int v) : value(v) {}
};

// Ідентифікатор
struct IdentifierExpr : Expression {
    std::string name;
    IdentifierExpr(const std::string& n) : name(n) {}
};

// Унарний вираз
struct UnaryExpr : Expression {
    std::string op;
    std::unique_ptr<Expression> operand;
    UnaryExpr(const std::string& o, std::unique_ptr<Expression> e)
        : op(o), operand(std::move(e)) {}
};

// Бінарний вираз
struct BinaryExpr : Expression {
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    BinaryExpr(const std::string& o, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

// Оператор (statement)
struct Statement : ASTNode {
    virtual ~Statement() = default;
};

// Присвоєння
struct AssignmentStmt : Statement {
    std::string variable;
    std::unique_ptr<Expression> expression;
    AssignmentStmt(const std::string& v, std::unique_ptr<Expression> e)
        : variable(v), expression(std::move(e)) {}
};

// Введення (Get)
struct GetStmt : Statement {
    std::string variable;
    GetStmt(const std::string& v) : variable(v) {}
};

// Виведення (Put)
struct PutStmt : Statement {
    std::unique_ptr<Expression> expression;
    PutStmt(std::unique_ptr<Expression> e) : expression(std::move(e)) {}
};

// Мітка
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

// Складений оператор (Begin-End)
struct BlockStmt : Statement {
    std::vector<std::unique_ptr<Statement>> statements;
};

// Програма
struct Program : ASTNode {
    std::string name;
    std::vector<std::string> variables;
    std::vector<std::unique_ptr<Statement>> statements;
};

// Синтаксичний аналізатор
class Parser {
public:
    Parser(const std::vector<Token>& tokens);

    // Парсинг програми
    std::unique_ptr<Program> parse();

    // Перевірити чи є помилки
    bool hasErrors() const { return !errors.empty(); }

    // Отримати список помилок
    const std::vector<std::string>& getErrors() const { return errors; }

    // Отримати таблицю символів
    SymbolTable& getSymbolTable() { return symbolTable; }

private:
    std::vector<Token> tokens;
    size_t current;
    std::vector<std::string> errors;
    SymbolTable symbolTable;

    // Допоміжні методи
    Token peek() const;
    Token previous() const;
    Token advance();
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    void addError(const std::string& message);
    void synchronize();

    // Парсинг структури програми
    void parseProgram(Program& program);
    void parseVarDeclaration(Program& program);
    void parseStatements(std::vector<std::unique_ptr<Statement>>& statements);

    // Парсинг операторів
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseAssignment();
    std::unique_ptr<Statement> parseGet();
    std::unique_ptr<Statement> parsePut();
    std::unique_ptr<Statement> parseIf();
    std::unique_ptr<Statement> parseGoto();
    std::unique_ptr<Statement> parseFor();
    std::unique_ptr<Statement> parseBlock();
    std::unique_ptr<Statement> parseLabel();

    // Парсинг виразів
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
