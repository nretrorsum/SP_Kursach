#ifndef CODEGEN_H
#define CODEGEN_H

#include <string>
#include <sstream>
#include "parser.h"

class CodeGenerator {
private:
    std::stringstream output;
    int indentLevel;

    void indent();
    void writeLine(const std::string& line);
    void generateStatement(Statement* stmt);
    std::string generateExpression(Expression* expr);
    std::string translateOperator(const std::string& op);

public:
    CodeGenerator();
    std::string generate(Program* program);
};

#endif // CODEGEN_H
