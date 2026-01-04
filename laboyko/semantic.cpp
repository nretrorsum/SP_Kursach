#include "semantic.h"
#include <limits>

SemanticAnalyzer::SemanticAnalyzer(SymbolTable& symTable) : symbolTable(symTable) {}

void SemanticAnalyzer::analyze(Program* program) {
    // Analyze all statements
    for (auto& stmt : program->statements) {
        analyzeStatement(stmt.get());
    }

    // Check label usage
    checkLabelUsage();
}

void SemanticAnalyzer::analyzeStatement(Statement* stmt) {
    if (auto assign = dynamic_cast<AssignStmt*>(stmt)) {
        // Check if variable is declared
        if (!symbolTable.isDeclared(assign->variable)) {
            errors.push_back("Line " + std::to_string(assign->line) +
                           ": Undeclared variable '" + assign->variable + "'");
        }
        analyzeExpression(assign->value.get());
    }
    else if (auto getStmt = dynamic_cast<GetStmt*>(stmt)) {
        if (!symbolTable.isDeclared(getStmt->variable)) {
            errors.push_back("Line " + std::to_string(getStmt->line) +
                           ": Undeclared variable '" + getStmt->variable + "'");
        }
    }
    else if (auto putStmt = dynamic_cast<PutStmt*>(stmt)) {
        analyzeExpression(putStmt->value.get());
    }
    else if (auto labelStmt = dynamic_cast<LabelStmt*>(stmt)) {
        definedLabels.insert(labelStmt->name);
    }
    else if (auto gotoStmt = dynamic_cast<GotoStmt*>(stmt)) {
        usedLabels.insert(gotoStmt->label);
    }
    else if (auto ifGotoStmt = dynamic_cast<IfGotoStmt*>(stmt)) {
        analyzeExpression(ifGotoStmt->condition.get());
        usedLabels.insert(ifGotoStmt->label);
    }
    else if (auto forStmt = dynamic_cast<ForStmt*>(stmt)) {
        // Check loop variable
        if (!symbolTable.isDeclared(forStmt->variable)) {
            errors.push_back("Line " + std::to_string(forStmt->line) +
                           ": Undeclared loop variable '" + forStmt->variable + "'");
        }
        analyzeExpression(forStmt->start.get());
        analyzeExpression(forStmt->end.get());

        // Analyze body
        for (auto& bodyStmt : forStmt->body) {
            analyzeStatement(bodyStmt.get());
        }
    }
}

void SemanticAnalyzer::analyzeExpression(Expression* expr) {
    if (auto ident = dynamic_cast<IdentifierExpr*>(expr)) {
        if (!symbolTable.isDeclared(ident->name)) {
            errors.push_back("Line " + std::to_string(ident->line) +
                           ": Undeclared variable '" + ident->name + "'");
        } else if (!symbolTable.isInitialized(ident->name)) {
            warnings.push_back("Line " + std::to_string(ident->line) +
                             ": Variable '" + ident->name + "' may be used uninitialized");
        }
    }
    else if (auto binary = dynamic_cast<BinaryExpr*>(expr)) {
        analyzeExpression(binary->left.get());
        analyzeExpression(binary->right.get());

        // Check for division by zero (if right is a constant 0)
        if ((binary->op == "Div" || binary->op == "Mod")) {
            if (auto num = dynamic_cast<NumberExpr*>(binary->right.get())) {
                if (num->value == 0) {
                    errors.push_back("Line " + std::to_string(binary->line) +
                                   ": Division by zero");
                }
            }
        }
    }
    else if (auto unary = dynamic_cast<UnaryExpr*>(expr)) {
        analyzeExpression(unary->operand.get());
    }
    else if (auto num = dynamic_cast<NumberExpr*>(expr)) {
        // Check Integer32_t range
        if (num->value < std::numeric_limits<int32_t>::min() ||
            num->value > std::numeric_limits<int32_t>::max()) {
            errors.push_back("Line " + std::to_string(num->line) +
                           ": Number out of Integer32_t range");
        }
    }
}

void SemanticAnalyzer::checkLabelUsage() {
    // Check for undefined labels
    for (const auto& label : usedLabels) {
        if (definedLabels.find(label) == definedLabels.end()) {
            errors.push_back("Undefined label '" + label + "'");
        }
    }

    // Warn about unused labels
    for (const auto& label : definedLabels) {
        if (usedLabels.find(label) == usedLabels.end()) {
            warnings.push_back("Label '" + label + "' is defined but never used");
        }
    }
}
