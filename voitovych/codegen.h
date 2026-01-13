/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V03 programming language
 * Variant: Voitovych Oleksandr Viktorovych
 *
 * File: codegen.h
 * Description: Code generator header file (translation to C)
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include <string>
#include <sstream>

class CodeGenerator {
public:
    CodeGenerator();

    // Generate program code
    std::string generate(const Program& program);

private:
    std::stringstream output;
    int indentLevel;

    // Helper methods
    void indent();
    void writeLine(const std::string& line);
    void write(const std::string& text);

    // Program parts generation
    void generateIncludes();
    void generateVariables(const std::vector<std::string>& variables);
    void generateStatements(const std::vector<std::unique_ptr<Statement>>& statements);

    // Statement generation
    void generateStatement(const Statement* stmt);
    void generateAssignment(const AssignmentStmt* stmt);
    void generateGet(const GetStmt* stmt);
    void generatePut(const PutStmt* stmt);
    void generateIfGoto(const IfGotoStmt* stmt);
    void generateGoto(const GotoStmt* stmt);
    void generateFor(const ForStmt* stmt);
    void generateBlock(const BlockStmt* stmt);
    void generateLabel(const LabelStmt* stmt);

    // Expression generation
    std::string generateExpression(const Expression* expr);
    std::string generateBinaryExpr(const BinaryExpr* expr);
    std::string generateUnaryExpr(const UnaryExpr* expr);

    // Convert V03 operators to C operators
    std::string translateOperator(const std::string& op);
};

#endif // CODEGEN_H
