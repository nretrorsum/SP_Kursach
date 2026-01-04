#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include <vector>
#include <set>
#include "parser.h"
#include "symbol_table.h"

class SemanticAnalyzer {
private:
    SymbolTable& symbolTable;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::set<std::string> usedLabels;
    std::set<std::string> definedLabels;

    void analyzeStatement(Statement* stmt);
    void analyzeExpression(Expression* expr);
    void checkLabelUsage();

public:
    SemanticAnalyzer(SymbolTable& symTable);
    void analyze(Program* program);
    const std::vector<std::string>& getErrors() const { return errors; }
    const std::vector<std::string>& getWarnings() const { return warnings; }
};

#endif // SEMANTIC_H
