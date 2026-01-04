/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
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
    void generateIf(const IfStmt* stmt);
    void generateWhile(const WhileStmt* stmt);
    void generateBlock(const BlockStmt* stmt);

    // Expression generation
    std::string generateExpression(const Expression* expr);
    std::string generateBinaryExpr(const BinaryExpr* expr);
    std::string generateUnaryExpr(const UnaryExpr* expr);

    // Convert O16 operators to C operators
    std::string translateOperator(const std::string& op);

    // Convert O16 identifier to C identifier (remove leading _)
    std::string translateIdentifier(const std::string& id);
};

#endif // CODEGEN_H
