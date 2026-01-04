/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V03 programming language
 * Variant: Voitovych Oleksandr Viktorovych
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
    SemanticAnalyzer(SymbolTable& symbolTable);

    // Analyze program
    void analyze(const Program& program);

    // Check if there are errors
    bool hasErrors() const { return !errors.empty(); }

    // Check if there are warnings
    bool hasWarnings() const { return !warnings.empty(); }

    // Get error list
    const std::vector<std::string>& getErrors() const { return errors; }

    // Get warning list
    const std::vector<std::string>& getWarnings() const { return warnings; }

private:
    SymbolTable& symbolTable;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;

    // Statement analysis
    void analyzeStatement(const Statement* stmt);
    void analyzeExpression(const Expression* expr);

    // Specific analysis
    void analyzeAssignment(const AssignmentStmt* stmt);
    void analyzeGet(const GetStmt* stmt);
    void analyzePut(const PutStmt* stmt);
    void analyzeIfGoto(const IfGotoStmt* stmt);
    void analyzeGoto(const GotoStmt* stmt);
    void analyzeFor(const ForStmt* stmt);
    void analyzeBlock(const BlockStmt* stmt);
    void analyzeLabel(const LabelStmt* stmt);

    // Label checking
    void checkLabels();

    // Add messages
    void addError(const std::string& message);
    void addWarning(const std::string& message);
};

#endif // SEMANTIC_H
