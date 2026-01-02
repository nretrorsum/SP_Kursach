#ifndef CODEGEN_H
#define CODEGEN_H

#include "token.h"
#include "symbol_table.h"
#include <vector>
#include <string>
#include <sstream>

class CodeGenerator {
private:
    vector<Token> tokens;
    int pos;
    SymbolTable& symbolTable;
    stringstream output;
    int indentLevel;

    Token current();
    Token peek(int offset);
    void advance();
    bool match(TokenType type);

    void indent();
    void emit(const string& code);
    void emitLine(const string& code);

    void generateProgram();
    void generateVariableDeclaration();
    void generateCompoundStatement();
    void generateStatement();
    void generateAssignment();
    void generateInput();
    void generateOutput();
    void generateIf();
    void generateFor();
    string generateExpression();

public:
    CodeGenerator(vector<Token> t, SymbolTable& st);
    string generate();
    bool saveToFile(const string& filename);
};

#endif
