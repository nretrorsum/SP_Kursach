/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
 *
 * File: semantic.h
 * Description: Semantic analyzer header file
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"
#include "symbol_table.h"
#include <vector>
#include <string>

class SemanticAnalyzer {
public:
    explicit SemanticAnalyzer(SymbolTable& symbolTable);

    // Main analysis method
    void analyze(const Program& program);

    // Error and warning handling
    bool hasErrors() const;
    bool hasWarnings() const;
    const std::vector<std::string>& getErrors() const;
    const std::vector<std::string>& getWarnings() const;

private:
    SymbolTable& symbolTable;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;

    // Analysis methods
    void analyzeStatement(const Statement* stmt);
    void analyzeAssignment(const AssignmentStmt* stmt);
    void analyzeGet(const GetStmt* stmt);
    void analyzePut(const PutStmt* stmt);
    void analyzeIf(const IfStmt* stmt);
    void analyzeWhile(const WhileStmt* stmt);
    void analyzeBlock(const BlockStmt* stmt);

    // Expression analysis
    void analyzeExpression(const Expression* expr);
    void analyzeBinaryExpr(const BinaryExpr* expr);
    void analyzeUnaryExpr(const UnaryExpr* expr);

    // Check for uninitialized variables
    void checkUninitializedVariables();

    // Error reporting
    void addError(const std::string& message);
    void addWarning(const std::string& message);
};

#endif // SEMANTIC_H
