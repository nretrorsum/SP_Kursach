/**
 * Курсовий проект з Системного Програмування
 * Тема: Розробка транслятора з вхідної мови програмування V07
 * Варіант: Войтович Олександр Вікторович
 *
 * Файл: semantic.h
 * Опис: Заголовний файл семантичного аналізатора
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

    // Аналіз програми
    void analyze(const Program& program);

    // Перевірити чи є помилки
    bool hasErrors() const { return !errors.empty(); }

    // Перевірити чи є попередження
    bool hasWarnings() const { return !warnings.empty(); }

    // Отримати список помилок
    const std::vector<std::string>& getErrors() const { return errors; }

    // Отримати список попереджень
    const std::vector<std::string>& getWarnings() const { return warnings; }

private:
    SymbolTable& symbolTable;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;

    // Аналіз операторів
    void analyzeStatement(const Statement* stmt);
    void analyzeExpression(const Expression* expr);

    // Специфічний аналіз
    void analyzeAssignment(const AssignmentStmt* stmt);
    void analyzeGet(const GetStmt* stmt);
    void analyzePut(const PutStmt* stmt);
    void analyzeIfGoto(const IfGotoStmt* stmt);
    void analyzeGoto(const GotoStmt* stmt);
    void analyzeFor(const ForStmt* stmt);
    void analyzeBlock(const BlockStmt* stmt);
    void analyzeLabel(const LabelStmt* stmt);

    // Перевірка міток
    void checkLabels();

    // Додавання повідомлень
    void addError(const std::string& message);
    void addWarning(const std::string& message);
};

#endif // SEMANTIC_H
