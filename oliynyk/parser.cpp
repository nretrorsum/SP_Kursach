/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
 *
 * File: parser.cpp
 * Description: Syntax analyzer implementation
 */

#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), current(0) {}

Token Parser::currentToken() const {
    if (current >= tokens.size()) {
        return Token(TokenType::END_OF_FILE, "", 0, 0);
    }
    return tokens[current];
}

Token Parser::peekToken() const {
    if (current + 1 >= tokens.size()) {
        return Token(TokenType::END_OF_FILE, "", 0, 0);
    }
    return tokens[current + 1];
}

void Parser::advance() {
    if (current < tokens.size()) {
        current++;
    }
}

bool Parser::check(TokenType type) const {
    return currentToken().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(TokenType type, const std::string& message) {
    if (!match(type)) {
        addError("Line " + std::to_string(currentToken().line) + ": " + message +
                " (found '" + currentToken().value + "')");
    }
}

std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>();
    parseProgram(*program);
    return program;
}

void Parser::parseProgram(Program& program) {
    // startprogram
    expect(TokenType::STARTPROGRAM, "Expected 'startprogram' at beginning of program");

    // variable declaration (optional section before startblok)
    if (check(TokenType::VARIABLE)) {
        parseVariableDeclaration(program);
    }

    // startblok
    expect(TokenType::STARTBLOK, "Expected 'startblok'");

    // statements
    parseStatements(program.statements);

    // endblok
    expect(TokenType::ENDBLOK, "Expected 'endblok' at end of program");
}

void Parser::parseVariableDeclaration(Program& program) {
    // variable
    expect(TokenType::VARIABLE, "Expected 'variable'");

    // Variable list
    do {
        if (check(TokenType::IDENTIFIER)) {
            std::string varName = currentToken().value;
            int line = currentToken().line;

            if (!symbolTable.addVariable(varName, line)) {
                addError("Line " + std::to_string(line) + ": Variable '" + varName + "' already declared");
            }

            program.variables.push_back(varName);
            advance();
        } else {
            addError("Line " + std::to_string(currentToken().line) + ": Expected identifier in variable declaration");
            advance();
        }
    } while (match(TokenType::COMMA));

    // Semicolon after variable list
    expect(TokenType::SEMICOLON, "Expected ';' after variable declaration");
}

void Parser::parseStatements(std::vector<std::unique_ptr<Statement>>& statements) {
    while (!check(TokenType::ENDBLOK) && !check(TokenType::END_OF_FILE) &&
           !check(TokenType::ELSE)) {
        auto stmt = parseStatement();
        if (stmt) {
            statements.push_back(std::move(stmt));
        }
    }
}

std::unique_ptr<Statement> Parser::parseStatement() {
    // Assignment: _VAR :: expression
    if (check(TokenType::IDENTIFIER)) {
        return parseAssignment();
    }

    // Get statement: get(_VAR)
    if (check(TokenType::GET)) {
        return parseGet();
    }

    // Put statement: put(expression)
    if (check(TokenType::PUT)) {
        return parsePut();
    }

    // If statement (Rust style)
    if (check(TokenType::IF)) {
        return parseIf();
    }

    // While statement (Rust style)
    if (check(TokenType::WHILE)) {
        return parseWhile();
    }

    // Block statement
    if (check(TokenType::STARTBLOK)) {
        return parseBlock();
    }

    addError("Line " + std::to_string(currentToken().line) +
            ": Unexpected token '" + currentToken().value + "'");
    advance();
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseAssignment() {
    std::string varName = currentToken().value;
    int line = currentToken().line;
    advance();

    expect(TokenType::ASSIGN, "Expected '::' in assignment");

    auto expr = parseExpression();

    expect(TokenType::SEMICOLON, "Expected ';' after assignment");

    // Mark variable as initialized
    symbolTable.setInitialized(varName);

    return std::make_unique<AssignmentStmt>(varName, std::move(expr), line);
}

std::unique_ptr<Statement> Parser::parseGet() {
    int line = currentToken().line;
    advance(); // skip 'get'

    expect(TokenType::LPAREN, "Expected '(' after 'get'");

    if (!check(TokenType::IDENTIFIER)) {
        addError("Line " + std::to_string(currentToken().line) + ": Expected identifier in 'get' statement");
        return nullptr;
    }

    std::string varName = currentToken().value;
    advance();

    expect(TokenType::RPAREN, "Expected ')' after identifier");
    expect(TokenType::SEMICOLON, "Expected ';' after 'get' statement");

    // Mark variable as initialized
    symbolTable.setInitialized(varName);

    return std::make_unique<GetStmt>(varName, line);
}

std::unique_ptr<Statement> Parser::parsePut() {
    int line = currentToken().line;
    advance(); // skip 'put'

    expect(TokenType::LPAREN, "Expected '(' after 'put'");

    auto expr = parseExpression();

    expect(TokenType::RPAREN, "Expected ')' after expression");
    expect(TokenType::SEMICOLON, "Expected ';' after 'put' statement");

    return std::make_unique<PutStmt>(std::move(expr), line);
}

std::unique_ptr<Statement> Parser::parseIf() {
    int line = currentToken().line;
    advance(); // skip 'if'

    auto condition = parseExpression();

    auto ifStmt = std::make_unique<IfStmt>(std::move(condition), line);

    // Then block: startblok ... endblok
    expect(TokenType::STARTBLOK, "Expected 'startblok' after if condition");
    parseStatements(ifStmt->thenBranch);
    expect(TokenType::ENDBLOK, "Expected 'endblok' after if body");

    // Optional else block
    if (match(TokenType::ELSE)) {
        expect(TokenType::STARTBLOK, "Expected 'startblok' after 'else'");
        parseStatements(ifStmt->elseBranch);
        expect(TokenType::ENDBLOK, "Expected 'endblok' after else body");
    }

    return ifStmt;
}

std::unique_ptr<Statement> Parser::parseWhile() {
    int line = currentToken().line;
    advance(); // skip 'while'

    auto condition = parseExpression();

    auto whileStmt = std::make_unique<WhileStmt>(std::move(condition), line);

    // Body: startblok ... endblok
    expect(TokenType::STARTBLOK, "Expected 'startblok' after while condition");
    parseStatements(whileStmt->body);
    expect(TokenType::ENDBLOK, "Expected 'endblok' after while body");

    return whileStmt;
}

std::unique_ptr<Statement> Parser::parseBlock() {
    int line = currentToken().line;
    advance(); // skip 'startblok'

    auto block = std::make_unique<BlockStmt>(line);
    parseStatements(block->statements);

    expect(TokenType::ENDBLOK, "Expected 'endblok'");

    return block;
}

// Expression parsing with precedence
std::unique_ptr<Expression> Parser::parseExpression() {
    return parseLogicalOr();
}

std::unique_ptr<Expression> Parser::parseLogicalOr() {
    auto left = parseLogicalAnd();

    while (check(TokenType::OR)) {
        std::string op = currentToken().value;
        int line = currentToken().line;
        advance();
        auto right = parseLogicalAnd();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line);
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseLogicalAnd() {
    auto left = parseComparison();

    while (check(TokenType::AND)) {
        std::string op = currentToken().value;
        int line = currentToken().line;
        advance();
        auto right = parseComparison();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line);
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseComparison() {
    auto left = parseAdditive();

    while (check(TokenType::EQ) || check(TokenType::NOTEQ) ||
           check(TokenType::LESS) || check(TokenType::GR)) {
        std::string op = currentToken().value;
        int line = currentToken().line;
        advance();
        auto right = parseAdditive();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line);
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseAdditive() {
    auto left = parseMultiplicative();

    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        std::string op = currentToken().value;
        int line = currentToken().line;
        advance();
        auto right = parseMultiplicative();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line);
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseMultiplicative() {
    auto left = parseUnary();

    while (check(TokenType::MULTIPLY) || check(TokenType::DIVIDE) || check(TokenType::MODULO)) {
        std::string op = currentToken().value;
        int line = currentToken().line;
        advance();
        auto right = parseUnary();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line);
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseUnary() {
    if (check(TokenType::NOT) || check(TokenType::MINUS)) {
        std::string op = currentToken().value;
        int line = currentToken().line;
        advance();
        auto operand = parseUnary();
        return std::make_unique<UnaryExpr>(op, std::move(operand), line);
    }

    return parsePrimary();
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    // Number
    if (check(TokenType::NUMBER)) {
        int value = std::stoi(currentToken().value);
        int line = currentToken().line;
        advance();
        return std::make_unique<NumberExpr>(value, line);
    }

    // Identifier
    if (check(TokenType::IDENTIFIER)) {
        std::string name = currentToken().value;
        int line = currentToken().line;
        advance();
        return std::make_unique<IdentifierExpr>(name, line);
    }

    // Parenthesized expression
    if (match(TokenType::LPAREN)) {
        auto expr = parseExpression();
        expect(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }

    addError("Line " + std::to_string(currentToken().line) +
            ": Expected expression, found '" + currentToken().value + "'");
    advance();
    return std::make_unique<NumberExpr>(0, currentToken().line);
}

bool Parser::hasErrors() const {
    return !errors.empty();
}

const std::vector<std::string>& Parser::getErrors() const {
    return errors;
}

SymbolTable& Parser::getSymbolTable() {
    return symbolTable;
}

void Parser::addError(const std::string& message) {
    errors.push_back(message);
}
