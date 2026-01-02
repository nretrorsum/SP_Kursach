#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include <vector>

class Parser {
private:
    vector<Token> tokens;
    int pos;

    Token current();
    void match(TokenType type);

    void parseProgram();
    void parseVariableDeclaration();
    void parseCompoundStatement();
    void parseStatement();
    void parseAssignment();
    void parseInput();
    void parseOutput();
    void parseIf();
    void parseFor();
    
    void parseExpression();
    void parseLogic();
    void parseComparison();
    void parseAddSub();
    void parseTerm();
    void parseFactor();

public:
    Parser(vector<Token> t);
    void parse();
};

#endif