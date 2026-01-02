/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V07 programming language
 * Variant: Voitovych Oleksandr Viktorovych
 *
 * File: parser.cpp
 * Description: Syntax analyzer implementation (recursive descent)
 */

#include "parser.h"
#include <sstream>
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), current(0) {}

Token Parser::peek() const {
    if (current >= tokens.size()) return Token(TokenType::END_OF_FILE, "", 0, 0);
    return tokens[current];
}

Token Parser::previous() const {
    if (current == 0) return Token(TokenType::UNKNOWN, "", 0, 0);
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return current >= tokens.size() || tokens[current].type == TokenType::END_OF_FILE;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    addError(message);
    return Token(TokenType::UNKNOWN, "", peek().line, peek().column);
}

void Parser::addError(const std::string& message) {
    std::stringstream ss;
    ss << "Syntax error [line " << peek().line << ", column " << peek().column << "]: " << message;
    errors.push_back(ss.str());
}

void Parser::synchronize() {
    advance();
    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;
        switch (peek().type) {
            case TokenType::IF:
            case TokenType::FOR:
            case TokenType::GET:
            case TokenType::PUT:
            case TokenType::GOTO:
            case TokenType::BEGIN:
            case TokenType::END:
                return;
            default:
                advance();
        }
    }
}

std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>();
    parseProgram(*program);
    return program;
}

void Parser::parseProgram(Program& program) {
    // Program Name;
    consume(TokenType::PROGRAM, "Expected 'Program'");
    Token nameToken = consume(TokenType::IDENTIFIER, "Expected program name");
    program.name = nameToken.value;
    consume(TokenType::SEMICOLON, "Expected ';' after program name");

    // Var Int16 variables;
    parseVarDeclaration(program);

    // Begin
    consume(TokenType::BEGIN, "Expected 'Begin'");

    // Statements
    parseStatements(program.statements);

    // End
    consume(TokenType::END, "Expected 'End'");
}

void Parser::parseVarDeclaration(Program& program) {
    // Var
    consume(TokenType::VAR, "Expected 'Var'");

    // Int16
    consume(TokenType::INT16, "Expected 'Int16'");

    // Variable list
    do {
        Token varToken = consume(TokenType::IDENTIFIER, "Expected variable name");
        if (varToken.type == TokenType::IDENTIFIER) {
            if (!symbolTable.addVariable(varToken.value, varToken.line)) {
                addError("Variable '" + varToken.value + "' is already declared");
            } else {
                program.variables.push_back(varToken.value);
            }
        }
    } while (match(TokenType::COMMA));

    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");
}

void Parser::parseStatements(std::vector<std::unique_ptr<Statement>>& statements) {
    while (!isAtEnd() && !check(TokenType::END) && !check(TokenType::NEXT)) {
        auto stmt = parseStatement();
        if (stmt) {
            statements.push_back(std::move(stmt));
        }

        // Optional semicolon after statement
        match(TokenType::SEMICOLON);
    }
}

std::unique_ptr<Statement> Parser::parseStatement() {
    // Check for label (IDENTIFIER followed by colon)
    if (check(TokenType::IDENTIFIER)) {
        // Look ahead to check if this is a label
        if (current + 1 < tokens.size() && tokens[current + 1].type == TokenType::COLON) {
            return parseLabel();
        }
        return parseAssignment();
    }

    if (check(TokenType::GET)) return parseGet();
    if (check(TokenType::PUT)) return parsePut();
    if (check(TokenType::IF)) return parseIf();
    if (check(TokenType::GOTO)) return parseGoto();
    if (check(TokenType::FOR)) return parseFor();
    if (check(TokenType::BEGIN)) return parseBlock();

    addError("Unknown statement: " + peek().value);
    synchronize();
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseLabel() {
    Token labelToken = advance(); // IDENTIFIER
    advance(); // :

    symbolTable.addLabel(labelToken.value, labelToken.line);
    return std::make_unique<LabelStmt>(labelToken.value);
}

std::unique_ptr<Statement> Parser::parseAssignment() {
    Token varToken = consume(TokenType::IDENTIFIER, "Expected variable name");
    consume(TokenType::ASSIGN, "Expected '::='");
    auto expr = parseExpression();

    if (!symbolTable.hasVariable(varToken.value)) {
        addError("Variable '" + varToken.value + "' is not declared");
    } else {
        symbolTable.setInitialized(varToken.value);
    }

    return std::make_unique<AssignmentStmt>(varToken.value, std::move(expr));
}

std::unique_ptr<Statement> Parser::parseGet() {
    advance(); // Get
    Token varToken = consume(TokenType::IDENTIFIER, "Expected variable name");

    if (!symbolTable.hasVariable(varToken.value)) {
        addError("Variable '" + varToken.value + "' is not declared");
    } else {
        symbolTable.setInitialized(varToken.value);
    }

    return std::make_unique<GetStmt>(varToken.value);
}

std::unique_ptr<Statement> Parser::parsePut() {
    advance(); // Put
    auto expr = parseExpression();
    return std::make_unique<PutStmt>(std::move(expr));
}

std::unique_ptr<Statement> Parser::parseIf() {
    advance(); // If
    auto condition = parseExpression();
    consume(TokenType::GOTO, "Expected 'Goto' after condition");
    Token labelToken = consume(TokenType::IDENTIFIER, "Expected label");

    symbolTable.useLabel(labelToken.value, labelToken.line);

    return std::make_unique<IfGotoStmt>(std::move(condition), labelToken.value);
}

std::unique_ptr<Statement> Parser::parseGoto() {
    advance(); // Goto
    Token labelToken = consume(TokenType::IDENTIFIER, "Expected label");

    symbolTable.useLabel(labelToken.value, labelToken.line);

    return std::make_unique<GotoStmt>(labelToken.value);
}

std::unique_ptr<Statement> Parser::parseFor() {
    advance(); // For
    Token varToken = consume(TokenType::IDENTIFIER, "Expected loop variable");

    if (!symbolTable.hasVariable(varToken.value)) {
        addError("Variable '" + varToken.value + "' is not declared");
    } else {
        symbolTable.setInitialized(varToken.value);
    }

    consume(TokenType::ASSIGN, "Expected '::='");
    auto start = parseExpression();
    consume(TokenType::TO, "Expected 'To'");
    auto end = parseExpression();

    auto forStmt = std::make_unique<ForStmt>(varToken.value, std::move(start), std::move(end));

    // Loop body (statements until Next)
    while (!isAtEnd() && !check(TokenType::NEXT)) {
        auto stmt = parseStatement();
        if (stmt) {
            forStmt->body.push_back(std::move(stmt));
        }
        match(TokenType::SEMICOLON);
    }

    consume(TokenType::NEXT, "Expected 'Next'");
    Token nextVar = consume(TokenType::IDENTIFIER, "Expected variable after 'Next'");

    if (nextVar.value != varToken.value) {
        addError("Variable after 'Next' must match loop variable '" + varToken.value + "'");
    }

    return forStmt;
}

std::unique_ptr<Statement> Parser::parseBlock() {
    advance(); // Begin
    auto block = std::make_unique<BlockStmt>();

    parseStatements(block->statements);

    consume(TokenType::END, "Expected 'End'");

    return block;
}

// Expression parsing with operator precedence

std::unique_ptr<Expression> Parser::parseExpression() {
    return parseOr();
}

std::unique_ptr<Expression> Parser::parseOr() {
    auto left = parseAnd();

    while (match(TokenType::OR)) {
        auto right = parseAnd();
        left = std::make_unique<BinaryExpr>("Or", std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseAnd() {
    auto left = parseEquality();

    while (match(TokenType::AND)) {
        auto right = parseEquality();
        left = std::make_unique<BinaryExpr>("And", std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseEquality() {
    auto left = parseComparison();

    while (match(TokenType::EG) || match(TokenType::NE)) {
        std::string op = previous().value;
        auto right = parseComparison();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseComparison() {
    auto left = parseAddition();

    while (match(TokenType::GT) || match(TokenType::LT)) {
        std::string op = previous().value;
        auto right = parseAddition();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseAddition() {
    auto left = parseMultiplication();

    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        std::string op = previous().value;
        auto right = parseMultiplication();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseMultiplication() {
    auto left = parseUnary();

    while (match(TokenType::MUL) || match(TokenType::DIV) || match(TokenType::MOD)) {
        std::string op = previous().value;
        auto right = parseUnary();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseUnary() {
    if (match(TokenType::NOT) || match(TokenType::MINUS)) {
        std::string op = previous().value;
        auto operand = parseUnary();
        return std::make_unique<UnaryExpr>(op, std::move(operand));
    }

    return parsePrimary();
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    // Number
    if (match(TokenType::NUMBER)) {
        return std::make_unique<NumberExpr>(std::stoi(previous().value));
    }

    // Identifier
    if (match(TokenType::IDENTIFIER)) {
        std::string name = previous().value;
        if (!symbolTable.hasVariable(name)) {
            addError("Variable '" + name + "' is not declared");
        } else if (!symbolTable.isInitialized(name)) {
            // Warning about uninitialized variable
            std::cerr << "Warning [line " << previous().line << "]: Variable '"
                      << name << "' may be uninitialized" << std::endl;
        }
        return std::make_unique<IdentifierExpr>(name);
    }

    // Parenthesized expression
    if (match(TokenType::LPAREN)) {
        auto expr = parseExpression();
        consume(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }

    addError("Expected expression, got: " + peek().value);
    return std::make_unique<NumberExpr>(0);
}
