/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V03 programming language
 * Variant: Voitovych Oleksandr Viktorovych
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
    // Convert V03 operators to C
    if (op == "Mul") return "*";
    if (op == "Div") return "/";
    if (op == "Mod") return "%";
    if (op == "Eg") return "==";
    if (op == "Ne") return "!=";
    if (op == ">>") return ">";
    if (op == "<<") return "<";
    if (op == "And") return "&&";
    if (op == "Or") return "||";
    if (op == "!") return "!";
    if (op == "+") return "+";
    if (op == "-") return "-";
    return op;
}

std::string CodeGenerator::generate(const Program& program) {
    output.str("");
    output.clear();

    // Comment with program information
    writeLine("/**");
    writeLine(" * Generated code from V03 language");
    writeLine(" * Program: " + program.name);
    writeLine(" * Variant: Voitovych Oleksandr Viktorovych");
    writeLine(" */");
    writeLine("");

    // Includes
    generateIncludes();

    // Main function
    writeLine("int main() {");
    indentLevel++;

    // Variables
    generateVariables(program.variables);
    writeLine("");

    // Statements
    generateStatements(program.statements);

    // Finish
    writeLine("");
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

    indent();
    output << "int16_t ";
    for (size_t i = 0; i < variables.size(); i++) {
        output << variables[i];
        if (i < variables.size() - 1) {
            output << ", ";
        }
    }
    output << ";\n";
}

void CodeGenerator::generateStatements(const std::vector<std::unique_ptr<Statement>>& statements) {
    for (const auto& stmt : statements) {
        generateStatement(stmt.get());
    }
}

void CodeGenerator::generateStatement(const Statement* stmt) {
    if (!stmt) return;

    if (auto assign = dynamic_cast<const AssignmentStmt*>(stmt)) {
        generateAssignment(assign);
    } else if (auto get = dynamic_cast<const GetStmt*>(stmt)) {
        generateGet(get);
    } else if (auto put = dynamic_cast<const PutStmt*>(stmt)) {
        generatePut(put);
    } else if (auto ifGoto = dynamic_cast<const IfGotoStmt*>(stmt)) {
        generateIfGoto(ifGoto);
    } else if (auto gotoStmt = dynamic_cast<const GotoStmt*>(stmt)) {
        generateGoto(gotoStmt);
    } else if (auto forStmt = dynamic_cast<const ForStmt*>(stmt)) {
        generateFor(forStmt);
    } else if (auto block = dynamic_cast<const BlockStmt*>(stmt)) {
        generateBlock(block);
    } else if (auto label = dynamic_cast<const LabelStmt*>(stmt)) {
        generateLabel(label);
    }
}

void CodeGenerator::generateAssignment(const AssignmentStmt* stmt) {
    indent();
    output << stmt->variable << " = " << generateExpression(stmt->expression.get()) << ";\n";
}

void CodeGenerator::generateGet(const GetStmt* stmt) {
    writeLine("scanf(\"%hd\", &" + stmt->variable + ");");
}

void CodeGenerator::generatePut(const PutStmt* stmt) {
    writeLine("printf(\"%d\\n\", " + generateExpression(stmt->expression.get()) + ");");
}

void CodeGenerator::generateIfGoto(const IfGotoStmt* stmt) {
    writeLine("if (" + generateExpression(stmt->condition.get()) + ") goto " + stmt->label + ";");
}

void CodeGenerator::generateGoto(const GotoStmt* stmt) {
    writeLine("goto " + stmt->label + ";");
}

void CodeGenerator::generateFor(const ForStmt* stmt) {
    std::string var = stmt->variable;
    std::string start = generateExpression(stmt->start.get());
    std::string end = generateExpression(stmt->end.get());

    writeLine("for (" + var + " = " + start + "; " + var + " <= " + end + "; " + var + "++) {");
    indentLevel++;

    for (const auto& bodyStmt : stmt->body) {
        generateStatement(bodyStmt.get());
    }

    indentLevel--;
    writeLine("}");
}

void CodeGenerator::generateBlock(const BlockStmt* stmt) {
    writeLine("{");
    indentLevel++;

    for (const auto& blockStmt : stmt->statements) {
        generateStatement(blockStmt.get());
    }

    indentLevel--;
    writeLine("}");
}

void CodeGenerator::generateLabel(const LabelStmt* stmt) {
    // Labels are output without indentation
    output << stmt->name << ":\n";
}

std::string CodeGenerator::generateExpression(const Expression* expr) {
    if (!expr) return "0";

    if (auto num = dynamic_cast<const NumberExpr*>(expr)) {
        return std::to_string(num->value);
    }

    if (auto id = dynamic_cast<const IdentifierExpr*>(expr)) {
        return id->name;
    }

    if (auto unary = dynamic_cast<const UnaryExpr*>(expr)) {
        return generateUnaryExpr(unary);
    }

    if (auto binary = dynamic_cast<const BinaryExpr*>(expr)) {
        return generateBinaryExpr(binary);
    }

    return "0";
}

std::string CodeGenerator::generateUnaryExpr(const UnaryExpr* expr) {
    std::string op = translateOperator(expr->op);
    std::string operand = generateExpression(expr->operand.get());

    return "(" + op + operand + ")";
}

std::string CodeGenerator::generateBinaryExpr(const BinaryExpr* expr) {
    std::string left = generateExpression(expr->left.get());
    std::string right = generateExpression(expr->right.get());
    std::string op = translateOperator(expr->op);

    return "(" + left + " " + op + " " + right + ")";
}
