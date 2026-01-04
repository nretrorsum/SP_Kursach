/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
 *
 * File: codegen.cpp
 * Description: Code generator implementation (translation to C)
 */

#include "codegen.h"

CodeGenerator::CodeGenerator() : indentLevel(0) {}

void CodeGenerator::indent() {
    for (int i = 0; i < indentLevel; i++) {
        output << "    ";
    }
}

void CodeGenerator::writeLine(const std::string& line) {
    indent();
    output << line << "\n";
}

void CodeGenerator::write(const std::string& text) {
    output << text;
}

std::string CodeGenerator::translateOperator(const std::string& op) {
    // Convert O16 operators to C
    if (op == "eq") return "==";
    if (op == "noteq") return "!=";
    if (op == "less") return "<";
    if (op == "gr") return ">";
    if (op == "and") return "&&";
    if (op == "or") return "||";
    if (op == "!") return "!";
    // Standard operators stay the same
    return op;
}

std::string CodeGenerator::translateIdentifier(const std::string& id) {
    // O16 identifiers start with _, C identifiers should not start with _
    // Convert _ABC to var_ABC for safety
    if (!id.empty() && id[0] == '_') {
        return "var" + id;
    }
    return id;
}

std::string CodeGenerator::generate(const Program& program) {
    output.str("");
    output.clear();

    // Comment with program information
    writeLine("/**");
    writeLine(" * Generated code from O16 language");
    writeLine(" * Variant: Oliynyk Bohdan Romanovych");
    writeLine(" */");
    writeLine("");

    // Includes
    generateIncludes();

    // Main function start
    writeLine("int main() {");
    indentLevel++;

    // Variable declarations
    generateVariables(program.variables);

    // Statements
    generateStatements(program.statements);

    // Return
    writeLine("return 0;");

    indentLevel--;
    writeLine("}");

    return output.str();
}

void CodeGenerator::generateIncludes() {
    writeLine("#include <stdio.h>");
    writeLine("#include <stdint.h>");
    writeLine("");
}

void CodeGenerator::generateVariables(const std::vector<std::string>& variables) {
    if (variables.empty()) return;

    writeLine("// Variable declarations (int_2 = int16_t)");
    for (const auto& var : variables) {
        writeLine("int16_t " + translateIdentifier(var) + " = 0;");
    }
    writeLine("");
}

void CodeGenerator::generateStatements(const std::vector<std::unique_ptr<Statement>>& statements) {
    for (const auto& stmt : statements) {
        generateStatement(stmt.get());
    }
}

void CodeGenerator::generateStatement(const Statement* stmt) {
    if (!stmt) return;

    switch (stmt->type) {
        case StmtType::ASSIGNMENT:
            generateAssignment(static_cast<const AssignmentStmt*>(stmt));
            break;
        case StmtType::GET:
            generateGet(static_cast<const GetStmt*>(stmt));
            break;
        case StmtType::PUT:
            generatePut(static_cast<const PutStmt*>(stmt));
            break;
        case StmtType::IF:
            generateIf(static_cast<const IfStmt*>(stmt));
            break;
        case StmtType::WHILE:
            generateWhile(static_cast<const WhileStmt*>(stmt));
            break;
        case StmtType::BLOCK:
            generateBlock(static_cast<const BlockStmt*>(stmt));
            break;
    }
}

void CodeGenerator::generateAssignment(const AssignmentStmt* stmt) {
    std::string expr = generateExpression(stmt->expression.get());
    writeLine(translateIdentifier(stmt->variable) + " = " + expr + ";");
}

void CodeGenerator::generateGet(const GetStmt* stmt) {
    writeLine("scanf(\"%hd\", &" + translateIdentifier(stmt->variable) + ");");
}

void CodeGenerator::generatePut(const PutStmt* stmt) {
    std::string expr = generateExpression(stmt->expression.get());
    writeLine("printf(\"%d\\n\", " + expr + ");");
}

void CodeGenerator::generateIf(const IfStmt* stmt) {
    std::string cond = generateExpression(stmt->condition.get());
    writeLine("if (" + cond + ") {");

    indentLevel++;
    for (const auto& s : stmt->thenBranch) {
        generateStatement(s.get());
    }
    indentLevel--;

    if (!stmt->elseBranch.empty()) {
        writeLine("} else {");
        indentLevel++;
        for (const auto& s : stmt->elseBranch) {
            generateStatement(s.get());
        }
        indentLevel--;
    }

    writeLine("}");
}

void CodeGenerator::generateWhile(const WhileStmt* stmt) {
    std::string cond = generateExpression(stmt->condition.get());
    writeLine("while (" + cond + ") {");

    indentLevel++;
    for (const auto& s : stmt->body) {
        generateStatement(s.get());
    }
    indentLevel--;

    writeLine("}");
}

void CodeGenerator::generateBlock(const BlockStmt* stmt) {
    writeLine("{");
    indentLevel++;
    for (const auto& s : stmt->statements) {
        generateStatement(s.get());
    }
    indentLevel--;
    writeLine("}");
}

std::string CodeGenerator::generateExpression(const Expression* expr) {
    if (!expr) return "0";

    switch (expr->type) {
        case ExprType::NUMBER: {
            auto numExpr = static_cast<const NumberExpr*>(expr);
            return std::to_string(numExpr->value);
        }
        case ExprType::IDENTIFIER: {
            auto idExpr = static_cast<const IdentifierExpr*>(expr);
            return translateIdentifier(idExpr->name);
        }
        case ExprType::BINARY:
            return generateBinaryExpr(static_cast<const BinaryExpr*>(expr));
        case ExprType::UNARY:
            return generateUnaryExpr(static_cast<const UnaryExpr*>(expr));
        default:
            return "0";
    }
}

std::string CodeGenerator::generateBinaryExpr(const BinaryExpr* expr) {
    std::string left = generateExpression(expr->left.get());
    std::string right = generateExpression(expr->right.get());
    std::string op = translateOperator(expr->op);

    return "(" + left + " " + op + " " + right + ")";
}

std::string CodeGenerator::generateUnaryExpr(const UnaryExpr* expr) {
    std::string operand = generateExpression(expr->operand.get());
    std::string op = translateOperator(expr->op);

    return "(" + op + operand + ")";
}
