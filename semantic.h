#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "token.h"
#include "symbol_table.h"
#include <vector>

class SemanticAnalyzer {
private:
    vector<Token> tokens;
    int pos;
    SymbolTable symbolTable;
    vector<string> errors;

    Token current();
    Token peek(int offset);
    void advance();
    bool match(TokenType type);

    void analyzeProgram();
    void analyzeVariableDeclaration();
    void analyzeCompoundStatement();
    void analyzeStatement();
    void analyzeAssignment();
    void analyzeInput();
    void analyzeOutput();
    void analyzeIf();
    void analyzeFor();
    void analyzeForBody();
    void analyzeExpression();
    void analyzeExpressionRecursive();

public:
    SemanticAnalyzer(vector<Token> t);
    bool analyze();
    void printErrors();
    void printSymbolTable();
    SymbolTable& getSymbolTable();
    bool hasErrors();
};

#endif
