#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens, SymbolTable& symTable)
    : tokens(tokens), pos(0), symbolTable(symTable) {}

Token& Parser::currentToken() {
    return tokens[pos];
}

Token& Parser::peekToken(int offset) {
    if (pos + offset < tokens.size()) {
        return tokens[pos + offset];
    }
    return tokens.back();
}

void Parser::advance() {
    if (pos < tokens.size() - 1) {
        pos++;
    }
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    return currentToken().type == type;
}

void Parser::expect(TokenType type, const std::string& message) {
    if (!match(type)) {
        errors.push_back("Line " + std::to_string(currentToken().line) + ": " + message +
                        " (got '" + currentToken().value + "')");
    }
}

std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>();

    // #Program name;
    expect(TokenType::HASH_PROGRAM, "Expected '#Program'");
    if (check(TokenType::IDENTIFIER)) {
        program->name = currentToken().value;
        advance();
    }
    expect(TokenType::SEMICOLON, "Expected ';' after program name");

    // Variable declarations (separate block)
    expect(TokenType::VARIABLE, "Expected 'Variable'");
    parseVariableDeclarations(*program);

    // Start - beginning of code block
    expect(TokenType::START, "Expected 'Start'");

    // Statements until Stop
    parseStatements(program->statements);

    expect(TokenType::STOP, "Expected 'Stop'");

    return program;
}

void Parser::parseVariableDeclarations(Program& program) {
    // Variables are separated by commas, no semicolon at end before operators
    while (check(TokenType::IDENTIFIER)) {
        std::string varName = currentToken().value;
        int line = currentToken().line;

        if (!symbolTable.declare(varName, line)) {
            errors.push_back("Line " + std::to_string(line) + ": Variable '" + varName + "' already declared");
        }
        program.variables.push_back(varName);
        advance();

        if (check(TokenType::COMMA)) {
            advance();
        } else {
            break;
        }
    }
}

void Parser::parseStatements(std::vector<std::unique_ptr<Statement>>& statements) {
    while (!check(TokenType::STOP) && !check(TokenType::NEXT) && !check(TokenType::END_OF_FILE)) {
        auto stmt = parseStatement();
        if (stmt) {
            statements.push_back(std::move(stmt));
        }
    }
}

std::unique_ptr<Statement> Parser::parseStatement() {
    // Check for label (identifier followed by colon was already handled in lexer)
    if (check(TokenType::LABEL)) {
        return parseLabel();
    }

    if (check(TokenType::IDENTIFIER)) {
        // Check if it's an assignment: IDENTIFIER >> expression
        if (peekToken().type == TokenType::ASSIGN) {
            return parseAssignment();
        }
        // Otherwise it might be a label definition (IDENTIFIER:)
        // The lexer should have handled this, but let's check
        return parseAssignment();
    }

    if (check(TokenType::GET)) {
        return parseGet();
    }

    if (check(TokenType::PUT)) {
        return parsePut();
    }

    if (check(TokenType::IF)) {
        return parseIfGoto();
    }

    if (check(TokenType::GOTO)) {
        return parseGoto();
    }

    if (check(TokenType::FOR)) {
        return parseFor();
    }

    // Skip unknown tokens
    errors.push_back("Line " + std::to_string(currentToken().line) +
                    ": Unexpected token '" + currentToken().value + "'");
    advance();
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseLabel() {
    int line = currentToken().line;
    std::string labelName = currentToken().value;
    advance(); // skip label

    if (!symbolTable.declareLabel(labelName, line)) {
        errors.push_back("Line " + std::to_string(line) + ": Label '" + labelName + "' already defined");
    }

    return std::make_unique<LabelStmt>(labelName, line);
}

std::unique_ptr<Statement> Parser::parseAssignment() {
    int line = currentToken().line;
    std::string varName = currentToken().value;
    advance(); // skip identifier

    expect(TokenType::ASSIGN, "Expected '>>' for assignment");

    auto value = parseExpression();
    expect(TokenType::SEMICOLON, "Expected ';' after assignment");

    symbolTable.setInitialized(varName);

    return std::make_unique<AssignStmt>(varName, std::move(value), line);
}

std::unique_ptr<Statement> Parser::parseGet() {
    int line = currentToken().line;
    advance(); // skip Get

    expect(TokenType::LPAREN, "Expected '(' after 'Get'");

    std::string varName = currentToken().value;
    expect(TokenType::IDENTIFIER, "Expected variable name");

    expect(TokenType::RPAREN, "Expected ')' after variable name");
    expect(TokenType::SEMICOLON, "Expected ';' after Get statement");

    symbolTable.setInitialized(varName);

    return std::make_unique<GetStmt>(varName, line);
}

std::unique_ptr<Statement> Parser::parsePut() {
    int line = currentToken().line;
    advance(); // skip Put

    expect(TokenType::LPAREN, "Expected '(' after 'Put'");

    auto value = parseExpression();

    expect(TokenType::RPAREN, "Expected ')' after expression");
    expect(TokenType::SEMICOLON, "Expected ';' after Put statement");

    return std::make_unique<PutStmt>(std::move(value), line);
}

std::unique_ptr<Statement> Parser::parseIfGoto() {
    int line = currentToken().line;
    advance(); // skip If

    auto condition = parseExpression();

    expect(TokenType::THEN, "Expected 'Then' after condition");
    expect(TokenType::GOTO, "Expected 'Goto' after 'Then'");

    std::string label = currentToken().value;
    expect(TokenType::IDENTIFIER, "Expected label name after 'Goto'");
    expect(TokenType::SEMICOLON, "Expected ';' after Goto statement");

    return std::make_unique<IfGotoStmt>(std::move(condition), label, line);
}

std::unique_ptr<Statement> Parser::parseGoto() {
    int line = currentToken().line;
    advance(); // skip Goto

    std::string label = currentToken().value;
    expect(TokenType::IDENTIFIER, "Expected label name after 'Goto'");
    expect(TokenType::SEMICOLON, "Expected ';' after Goto statement");

    return std::make_unique<GotoStmt>(label, line);
}

std::unique_ptr<Statement> Parser::parseFor() {
    int line = currentToken().line;
    advance(); // skip For

    std::string varName = currentToken().value;
    expect(TokenType::IDENTIFIER, "Expected loop variable");

    expect(TokenType::ASSIGN, "Expected '>>' after loop variable");

    auto startExpr = parseExpression();

    expect(TokenType::TO, "Expected 'To' after start value");

    auto endExpr = parseExpression();

    auto forStmt = std::make_unique<ForStmt>(varName, std::move(startExpr), std::move(endExpr), line);

    symbolTable.setInitialized(varName);

    // Parse body until Next
    parseStatements(forStmt->body);

    expect(TokenType::NEXT, "Expected 'Next' to close For loop");

    // Check if Next is followed by variable name (optional)
    if (check(TokenType::IDENTIFIER)) {
        if (currentToken().value != varName) {
            errors.push_back("Line " + std::to_string(currentToken().line) +
                           ": 'Next' variable doesn't match 'For' variable");
        }
        advance();
    }

    expect(TokenType::SEMICOLON, "Expected ';' after Next");

    return forStmt;
}

// Expression parsing with operator precedence

std::unique_ptr<Expression> Parser::parseExpression() {
    return parseLogicalOr();
}

std::unique_ptr<Expression> Parser::parseLogicalOr() {
    auto left = parseLogicalAnd();
    while (check(TokenType::OR)) {
        int line = currentToken().line;
        advance();
        auto right = parseLogicalAnd();
        left = std::make_unique<BinaryExpr>("Or", std::move(left), std::move(right), line);
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseLogicalAnd() {
    auto left = parseComparison();
    while (check(TokenType::AND)) {
        int line = currentToken().line;
        advance();
        auto right = parseComparison();
        left = std::make_unique<BinaryExpr>("And", std::move(left), std::move(right), line);
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseComparison() {
    auto left = parseAddSub();
    while (check(TokenType::EQ) || check(TokenType::NE) ||
           check(TokenType::NOT_GREATER) || check(TokenType::NOT_LESS)) {
        int line = currentToken().line;
        std::string op = currentToken().value;
        advance();
        auto right = parseAddSub();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line);
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseAddSub() {
    auto left = parseMulDiv();
    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        int line = currentToken().line;
        std::string op = currentToken().value;
        advance();
        auto right = parseMulDiv();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line);
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseMulDiv() {
    auto left = parseUnary();
    while (check(TokenType::MUL) || check(TokenType::DIV) || check(TokenType::MOD)) {
        int line = currentToken().line;
        std::string op = currentToken().value;
        advance();
        auto right = parseUnary();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line);
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseUnary() {
    if (check(TokenType::NOT) || check(TokenType::MINUS)) {
        int line = currentToken().line;
        std::string op = currentToken().value;
        advance();
        auto operand = parseUnary();
        return std::make_unique<UnaryExpr>(op, std::move(operand), line);
    }
    return parsePrimary();
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    int line = currentToken().line;

    if (check(TokenType::NUMBER)) {
        int value = std::stoi(currentToken().value);
        advance();
        return std::make_unique<NumberExpr>(value, line);
    }

    if (check(TokenType::IDENTIFIER)) {
        std::string name = currentToken().value;
        advance();
        return std::make_unique<IdentifierExpr>(name, line);
    }

    if (check(TokenType::LPAREN)) {
        advance();
        auto expr = parseExpression();
        expect(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }

    errors.push_back("Line " + std::to_string(line) + ": Expected expression");
    return std::make_unique<NumberExpr>(0, line);
}
