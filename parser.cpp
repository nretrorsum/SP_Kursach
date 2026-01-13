#include "parser.h"
#include <cstdlib>

Parser::Parser(vector<Token> t) : tokens(t), pos(0) {}

Token Parser::current() {
    return tokens[pos];
}

void Parser::match(TokenType type) {
    if (current().type == type) {
        pos++;
    } else {
        cout << "Syntax Error at line " << current().line 
             << ": Expected token type " << tokenTypeToString(type) 
             << ", but got " << current().value << endl;
        exit(1);
    }
}

void Parser::parse() {
    parseProgram();
}

void Parser::parseProgram() {
    cout << "Parsing Program..." << endl;
    match(K_PROGRAM);
    match(IDENTIFIER);
    match(SEMICOLON);

    match(K_VAR);
    parseVariableDeclaration();
    // No semicolon after variable declaration according to the variant

    parseCompoundStatement();

    if (current().type != END_OF_FILE) {
         if (current().type == SEMICOLON) match(SEMICOLON);
    }

    cout << ">>> Syntax Analysis Completed Successfully! <<<" << endl;
}

void Parser::parseVariableDeclaration() {
    match(K_INT16_T);
    match(IDENTIFIER);
    while (current().type == COMMA) {
        match(COMMA);
        match(IDENTIFIER);
    }
}

void Parser::parseCompoundStatement() {
    match(K_START);
    while (current().type != K_FINISH && current().type != END_OF_FILE) {
        parseStatement();
        // Semicolon after if/for is optional (they end with 'finish')
        // For assignment/get/put, semicolon is now required in respective functions
        if (current().type == SEMICOLON) {
            match(SEMICOLON);
        }
    }
    match(K_FINISH);
}

void Parser::parseStatement() {
    TokenType t = current().type;
    
    if (t == IDENTIFIER) {
        parseAssignment();
    } else if (t == K_GET) {
        parseInput();
    } else if (t == K_PUT) {
        parseOutput();
    } else if (t == K_IF) {
        parseIf();
    } else if (t == K_FOR) {
        parseFor();
    } else if (t == K_START) {
        parseCompoundStatement();
    } else {
        cout << "Syntax Error line " << current().line << ": Unexpected token " << current().value << endl;
        exit(1);
    }
}

void Parser::parseAssignment() {
    match(IDENTIFIER);
    match(OP_ASSIGN);
    parseExpression();
    match(SEMICOLON); // Semicolon is required after assignment
}

void Parser::parseInput() {
    match(K_GET);
    match(IDENTIFIER);
    match(SEMICOLON); // Semicolon is required after get
}

void Parser::parseOutput() {
    match(K_PUT);
    parseExpression();
    match(SEMICOLON); // Semicolon is required after put
}

void Parser::parseIf() {
    match(K_IF);
    parseExpression();
    parseCompoundStatement();
    if (current().type == K_ELSE) {
        match(K_ELSE);
        parseCompoundStatement();
    }
}

void Parser::parseFor() {
    match(K_FOR);
    match(IDENTIFIER);
    match(K_IN);
    parseExpression();
    match(RANGE);
    parseExpression();
    parseForBody();
}

void Parser::parseForBody() {
    match(LBRACE);
    while (current().type != RBRACE && current().type != END_OF_FILE) {
        parseStatement();
        if (current().type == SEMICOLON) {
            match(SEMICOLON);
        }
    }
    match(RBRACE);
}

void Parser::parseExpression() { parseLogic(); }

void Parser::parseLogic() {
    parseComparison();
    while (current().type == OP_AND || current().type == OP_OR) {
        pos++; parseComparison();
    }
}

void Parser::parseComparison() {
    parseAddSub();
    if (current().type == OP_GE || current().type == OP_LE ||
        current().type == OP_EG || current().type == OP_NE) {
        pos++; parseAddSub();
    }
}

void Parser::parseAddSub() {
    parseTerm();
    while (current().type == OP_PLUS || current().type == OP_MINUS) {
        pos++; parseTerm();
    }
}

void Parser::parseTerm() {
    parseFactor();
    while (current().type == OP_MUL || current().type == OP_DIV || current().type == OP_MOD) {
        pos++; parseFactor();
    }
}

void Parser::parseFactor() {
    TokenType t = current().type;
    if (t == IDENTIFIER || t == NUMBER) {
        pos++;
    } else if (t == LPAREN) {
        match(LPAREN);
        parseExpression();
        match(RPAREN);
    } else if (t == OP_NOT) {
        match(OP_NOT);
        parseFactor();
    } else {
        cout << "Syntax Error line " << current().line << ": Expected Number or ID, got " << current().value << endl;
        exit(1);
    }
}