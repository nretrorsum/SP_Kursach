/**
 * Курсовий проект з Системного Програмування
 * Тема: Розробка транслятора з вхідної мови програмування V07
 * Варіант: Войтович Олександр Вікторович
 *
 * Файл: semantic.cpp
 * Опис: Реалізація семантичного аналізатора
 */

#include "semantic.h"
#include <sstream>

SemanticAnalyzer::SemanticAnalyzer(SymbolTable& symbolTable)
    : symbolTable(symbolTable) {}

void SemanticAnalyzer::addError(const std::string& message) {
    errors.push_back("Семантична помилка: " + message);
}

void SemanticAnalyzer::addWarning(const std::string& message) {
    warnings.push_back("Попередження: " + message);
}

void SemanticAnalyzer::analyze(const Program& program) {
    // Аналіз всіх операторів
    for (const auto& stmt : program.statements) {
        analyzeStatement(stmt.get());
    }

    // Перевірка міток (чи всі використані мітки визначені)
    checkLabels();

    // Перевірка на невикористані змінні
    for (const auto& varName : symbolTable.getVariableOrder()) {
        auto var = symbolTable.getVariable(varName);
        if (var && !var->isInitialized) {
            addWarning("Змінна '" + varName + "' оголошена, але не ініціалізована");
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
        // Перевірка діапазону числа
        if (num->value < -32768 || num->value > 32767) {
            addError("Число " + std::to_string(num->value) + " виходить за межі діапазону Int16");
        }
    } else if (auto id = dynamic_cast<const IdentifierExpr*>(expr)) {
        // Перевірка чи змінна оголошена
        if (!symbolTable.hasVariable(id->name)) {
            addError("Змінна '" + id->name + "' не оголошена");
        }
    } else if (auto unary = dynamic_cast<const UnaryExpr*>(expr)) {
        analyzeExpression(unary->operand.get());
    } else if (auto binary = dynamic_cast<const BinaryExpr*>(expr)) {
        analyzeExpression(binary->left.get());
        analyzeExpression(binary->right.get());

        // Перевірка на ділення на нуль (якщо праий операнд - константа 0)
        if (binary->op == "Div" || binary->op == "Mod") {
            if (auto numRight = dynamic_cast<const NumberExpr*>(binary->right.get())) {
                if (numRight->value == 0) {
                    addError("Ділення на нуль");
                }
            }
        }
    }
}

void SemanticAnalyzer::analyzeAssignment(const AssignmentStmt* stmt) {
    // Перевірка чи змінна оголошена
    if (!symbolTable.hasVariable(stmt->variable)) {
        addError("Змінна '" + stmt->variable + "' не оголошена");
    }

    // Аналіз виразу
    analyzeExpression(stmt->expression.get());
}

void SemanticAnalyzer::analyzeGet(const GetStmt* stmt) {
    if (!symbolTable.hasVariable(stmt->variable)) {
        addError("Змінна '" + stmt->variable + "' не оголошена");
    }
}

void SemanticAnalyzer::analyzePut(const PutStmt* stmt) {
    analyzeExpression(stmt->expression.get());
}

void SemanticAnalyzer::analyzeIfGoto(const IfGotoStmt* stmt) {
    // Аналіз умови
    analyzeExpression(stmt->condition.get());

    // Мітка буде перевірена в checkLabels()
}

void SemanticAnalyzer::analyzeGoto(const GotoStmt* stmt) {
    // Мітка буде перевірена в checkLabels()
}

void SemanticAnalyzer::analyzeFor(const ForStmt* stmt) {
    // Перевірка змінної циклу
    if (!symbolTable.hasVariable(stmt->variable)) {
        addError("Змінна циклу '" + stmt->variable + "' не оголошена");
    }

    // Аналіз меж циклу
    analyzeExpression(stmt->start.get());
    analyzeExpression(stmt->end.get());

    // Аналіз тіла циклу
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
    // Мітки вже оброблені в парсері
}

void SemanticAnalyzer::checkLabels() {
    // Перевірка чи всі використані мітки визначені
    auto undefinedLabels = symbolTable.getUndefinedLabels();
    for (const auto& label : undefinedLabels) {
        addError("Мітка '" + label + "' використовується, але не визначена");
    }
}
