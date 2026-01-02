/**
 * Курсовий проект з Системного Програмування
 * Тема: Розробка транслятора з вхідної мови програмування V07
 * Варіант: Войтович Олександр Вікторович
 *
 * Файл: codegen.h
 * Опис: Заголовний файл генератора коду (трансляція в C)
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include <string>
#include <sstream>

class CodeGenerator {
public:
    CodeGenerator();

    // Генерація коду програми
    std::string generate(const Program& program);

private:
    std::stringstream output;
    int indentLevel;

    // Допоміжні методи
    void indent();
    void writeLine(const std::string& line);
    void write(const std::string& text);

    // Генерація частин програми
    void generateIncludes();
    void generateVariables(const std::vector<std::string>& variables);
    void generateStatements(const std::vector<std::unique_ptr<Statement>>& statements);

    // Генерація операторів
    void generateStatement(const Statement* stmt);
    void generateAssignment(const AssignmentStmt* stmt);
    void generateGet(const GetStmt* stmt);
    void generatePut(const PutStmt* stmt);
    void generateIfGoto(const IfGotoStmt* stmt);
    void generateGoto(const GotoStmt* stmt);
    void generateFor(const ForStmt* stmt);
    void generateBlock(const BlockStmt* stmt);
    void generateLabel(const LabelStmt* stmt);

    // Генерація виразів
    std::string generateExpression(const Expression* expr);
    std::string generateBinaryExpr(const BinaryExpr* expr);
    std::string generateUnaryExpr(const UnaryExpr* expr);

    // Перетворення операторів V07 в оператори C
    std::string translateOperator(const std::string& op);
};

#endif // CODEGEN_H
