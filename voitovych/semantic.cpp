/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V07 programming language
 * Variant: Voitovych Oleksandr Viktorovych
 *
 * File: semantic.cpp
 * Description: Semantic analyzer implementation
 */

#include "semantic.h"
#include <sstream>

SemanticAnalyzer::SemanticAnalyzer(SymbolTable& symbolTable)
    : symbolTable(symbolTable) {}

void SemanticAnalyzer::addError(const std::string& message) {
    errors.push_back("Semantic error: " + message);
}

void SemanticAnalyzer::addWarning(const std::string& message) {
    warnings.push_back("Warning: " + message);
}

void SemanticAnalyzer::analyze(const Program& program) {
    // Analyze all statements
    for (const auto& stmt : program.statements) {
        analyzeStatement(stmt.get());
    }

    // Check labels (verify all used labels are defined)
    checkLabels();

    // Check for unused variables
    for (const auto& varName : symbolTable.getVariableOrder()) {
        auto var = symbolTable.getVariable(varName);
        if (var && !var->isInitialized) {
            addWarning("Variable '" + varName + "' is declared but not initialized");
        }
    }
}

void SemanticAnalyzer::analyzeStatement(const Statement* stmt) {
    if (!stmt) return;

    if (auto assign = dynamic_cast<const AssignmentStmt*>(stmt)) {
        analyzeAssignment(assign);
    } else if (auto get = dynamic_cast<const GetStmt*>(stmt)) {
        analyzeGet(get);
    } else if (auto put = dynamic_cast<const PutStmt*>(stmt)) {
        analyzePut(put);
    } else if (auto ifGoto = dynamic_cast<const IfGotoStmt*>(stmt)) {
        analyzeIfGoto(ifGoto);
    } else if (auto gotoStmt = dynamic_cast<const GotoStmt*>(stmt)) {
        analyzeGoto(gotoStmt);
    } else if (auto forStmt = dynamic_cast<const ForStmt*>(stmt)) {
        analyzeFor(forStmt);
    } else if (auto block = dynamic_cast<const BlockStmt*>(stmt)) {
        analyzeBlock(block);
    } else if (auto label = dynamic_cast<const LabelStmt*>(stmt)) {
        analyzeLabel(label);
    }
}

void SemanticAnalyzer::analyzeExpression(const Expression* expr) {
    if (!expr) return;

    if (auto num = dynamic_cast<const NumberExpr*>(expr)) {
        // Check number range
        if (num->value < -32768 || num->value > 32767) {
            addError("Number " + std::to_string(num->value) + " is out of Int16 range");
        }
    } else if (auto id = dynamic_cast<const IdentifierExpr*>(expr)) {
        // Check if variable is declared
        if (!symbolTable.hasVariable(id->name)) {
            addError("Variable '" + id->name + "' is not declared");
        }
    } else if (auto unary = dynamic_cast<const UnaryExpr*>(expr)) {
        analyzeExpression(unary->operand.get());
    } else if (auto binary = dynamic_cast<const BinaryExpr*>(expr)) {
        analyzeExpression(binary->left.get());
        analyzeExpression(binary->right.get());

        // Check for division by zero (if right operand is constant 0)
        if (binary->op == "Div" || binary->op == "Mod") {
            if (auto numRight = dynamic_cast<const NumberExpr*>(binary->right.get())) {
                if (numRight->value == 0) {
                    addError("Division by zero");
                }
            }
        }
    }
}

void SemanticAnalyzer::analyzeAssignment(const AssignmentStmt* stmt) {
    // Check if variable is declared
    if (!symbolTable.hasVariable(stmt->variable)) {
        addError("Variable '" + stmt->variable + "' is not declared");
    }

    // Analyze expression
    analyzeExpression(stmt->expression.get());
}

void SemanticAnalyzer::analyzeGet(const GetStmt* stmt) {
    if (!symbolTable.hasVariable(stmt->variable)) {
        addError("Variable '" + stmt->variable + "' is not declared");
    }
}

void SemanticAnalyzer::analyzePut(const PutStmt* stmt) {
    analyzeExpression(stmt->expression.get());
}

void SemanticAnalyzer::analyzeIfGoto(const IfGotoStmt* stmt) {
    // Analyze condition
    analyzeExpression(stmt->condition.get());

    // Label will be checked in checkLabels()
}

void SemanticAnalyzer::analyzeGoto(const GotoStmt* stmt) {
    // Label will be checked in checkLabels()
}

void SemanticAnalyzer::analyzeFor(const ForStmt* stmt) {
    // Check loop variable
    if (!symbolTable.hasVariable(stmt->variable)) {
        addError("Loop variable '" + stmt->variable + "' is not declared");
    }

    // Analyze loop bounds
    analyzeExpression(stmt->start.get());
    analyzeExpression(stmt->end.get());

    // Analyze loop body
    for (const auto& bodyStmt : stmt->body) {
        analyzeStatement(bodyStmt.get());
    }
}

void SemanticAnalyzer::analyzeBlock(const BlockStmt* stmt) {
    for (const auto& blockStmt : stmt->statements) {
        analyzeStatement(blockStmt.get());
    }
}

void SemanticAnalyzer::analyzeLabel(const LabelStmt* stmt) {
    // Labels are already processed in parser
}

void SemanticAnalyzer::checkLabels() {
    // Check if all used labels are defined
    auto undefinedLabels = symbolTable.getUndefinedLabels();
    for (const auto& label : undefinedLabels) {
        addError("Label '" + label + "' is used but not defined");
    }
}
