/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
 *
 * File: semantic.cpp
 * Description: Semantic analyzer implementation
 */

#include "semantic.h"
#include <sstream>

SemanticAnalyzer::SemanticAnalyzer(SymbolTable& symbolTable)
    : symbolTable(symbolTable) {}

void SemanticAnalyzer::analyze(const Program& program) {
    // Analyze all statements
    for (const auto& stmt : program.statements) {
        analyzeStatement(stmt.get());
    }

    // Check for unused variables
    checkUninitializedVariables();
}

void SemanticAnalyzer::analyzeStatement(const Statement* stmt) {
    if (!stmt) return;

    switch (stmt->type) {
        case StmtType::ASSIGNMENT:
            analyzeAssignment(static_cast<const AssignmentStmt*>(stmt));
            break;
        case StmtType::GET:
            analyzeGet(static_cast<const GetStmt*>(stmt));
            break;
        case StmtType::PUT:
            analyzePut(static_cast<const PutStmt*>(stmt));
            break;
        case StmtType::IF:
            analyzeIf(static_cast<const IfStmt*>(stmt));
            break;
        case StmtType::WHILE:
            analyzeWhile(static_cast<const WhileStmt*>(stmt));
            break;
        case StmtType::BLOCK:
            analyzeBlock(static_cast<const BlockStmt*>(stmt));
            break;
    }
}

void SemanticAnalyzer::analyzeAssignment(const AssignmentStmt* stmt) {
    // Check if variable is declared
    if (!symbolTable.hasVariable(stmt->variable)) {
        addError("Line " + std::to_string(stmt->line) +
                ": Undeclared variable '" + stmt->variable + "'");
    }

    // Analyze expression
    analyzeExpression(stmt->expression.get());
}

void SemanticAnalyzer::analyzeGet(const GetStmt* stmt) {
    // Check if variable is declared
    if (!symbolTable.hasVariable(stmt->variable)) {
        addError("Line " + std::to_string(stmt->line) +
                ": Undeclared variable '" + stmt->variable + "'");
    }
}

void SemanticAnalyzer::analyzePut(const PutStmt* stmt) {
    analyzeExpression(stmt->expression.get());
}

void SemanticAnalyzer::analyzeIf(const IfStmt* stmt) {
    // Analyze condition
    analyzeExpression(stmt->condition.get());

    // Analyze then branch
    for (const auto& s : stmt->thenBranch) {
        analyzeStatement(s.get());
    }

    // Analyze else branch
    for (const auto& s : stmt->elseBranch) {
        analyzeStatement(s.get());
    }
}

void SemanticAnalyzer::analyzeWhile(const WhileStmt* stmt) {
    // Analyze condition
    analyzeExpression(stmt->condition.get());

    // Analyze body
    for (const auto& s : stmt->body) {
        analyzeStatement(s.get());
    }
}

void SemanticAnalyzer::analyzeBlock(const BlockStmt* stmt) {
    for (const auto& s : stmt->statements) {
        analyzeStatement(s.get());
    }
}

void SemanticAnalyzer::analyzeExpression(const Expression* expr) {
    if (!expr) return;

    switch (expr->type) {
        case ExprType::NUMBER:
            // Nothing to check for literals
            break;
        case ExprType::IDENTIFIER: {
            auto idExpr = static_cast<const IdentifierExpr*>(expr);
            if (!symbolTable.hasVariable(idExpr->name)) {
                addError("Line " + std::to_string(expr->line) +
                        ": Undeclared variable '" + idExpr->name + "'");
            } else if (!symbolTable.isInitialized(idExpr->name)) {
                addWarning("Line " + std::to_string(expr->line) +
                          ": Variable '" + idExpr->name + "' may be used before initialization");
            }
            break;
        }
        case ExprType::BINARY:
            analyzeBinaryExpr(static_cast<const BinaryExpr*>(expr));
            break;
        case ExprType::UNARY:
            analyzeUnaryExpr(static_cast<const UnaryExpr*>(expr));
            break;
    }
}

void SemanticAnalyzer::analyzeBinaryExpr(const BinaryExpr* expr) {
    analyzeExpression(expr->left.get());
    analyzeExpression(expr->right.get());
}

void SemanticAnalyzer::analyzeUnaryExpr(const UnaryExpr* expr) {
    analyzeExpression(expr->operand.get());
}

void SemanticAnalyzer::checkUninitializedVariables() {
    for (const auto& varName : symbolTable.getVariableOrder()) {
        auto var = symbolTable.getVariable(varName);
        if (var && !var->isInitialized) {
            addWarning("Variable '" + varName + "' declared but never assigned a value");
        }
    }
}

bool SemanticAnalyzer::hasErrors() const {
    return !errors.empty();
}

bool SemanticAnalyzer::hasWarnings() const {
    return !warnings.empty();
}

const std::vector<std::string>& SemanticAnalyzer::getErrors() const {
    return errors;
}

const std::vector<std::string>& SemanticAnalyzer::getWarnings() const {
    return warnings;
}

void SemanticAnalyzer::addError(const std::string& message) {
    errors.push_back(message);
}

void SemanticAnalyzer::addWarning(const std::string& message) {
    warnings.push_back(message);
}
