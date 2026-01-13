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

std::string CodeGenerator::translateOperator(const std::string& op) {
    // Arithmetic
    if (op == "Mul") return "*";
    if (op == "Div") return "/";
    if (op == "Mod") return "%";

    // Comparison
    if (op == "=") return "==";
    if (op == "<>") return "!=";
    if (op == "!>") return "<=";  // not greater means <=
    if (op == "!<") return ">=";  // not less means >=

    // Logical
    if (op == "And") return "&&";
    if (op == "Or") return "||";
    if (op == "!") return "!";

    return op;
}

std::string CodeGenerator::generate(Program* program) {
    // Header
    output << "#include <stdio.h>\n";
    output << "#include <stdint.h>\n\n";

    // Main function
    output << "int main() {\n";
    indentLevel++;

    // Variable declarations
    for (const auto& var : program->variables) {
        writeLine("int32_t " + var + " = 0;");
    }

    if (!program->variables.empty()) {
        output << "\n";
    }

    // Statements
    for (auto& stmt : program->statements) {
        generateStatement(stmt.get());
    }

    // Return
    output << "\n";
    writeLine("return 0;");

    indentLevel--;
    output << "}\n";

    return output.str();
}

void CodeGenerator::generateStatement(Statement* stmt) {
    if (auto assign = dynamic_cast<AssignStmt*>(stmt)) {
        std::string expr = generateExpression(assign->value.get());
        writeLine(assign->variable + " = " + expr + ";");
    }
    else if (auto getStmt = dynamic_cast<GetStmt*>(stmt)) {
        writeLine("scanf(\"%d\", &" + getStmt->variable + ");");
    }
    else if (auto putStmt = dynamic_cast<PutStmt*>(stmt)) {
        std::string expr = generateExpression(putStmt->value.get());
        writeLine("printf(\"%d\\n\", " + expr + ");");
    }
    else if (auto labelStmt = dynamic_cast<LabelStmt*>(stmt)) {
        // Labels in C need to be at indentLevel 0 or have a statement after
        indentLevel--;
        writeLine(labelStmt->name + ":;");
        indentLevel++;
    }
    else if (auto gotoStmt = dynamic_cast<GotoStmt*>(stmt)) {
        writeLine("goto " + gotoStmt->label + ";");
    }
    else if (auto ifGotoStmt = dynamic_cast<IfGotoStmt*>(stmt)) {
        std::string cond = generateExpression(ifGotoStmt->condition.get());
        writeLine("if (" + cond + ") goto " + ifGotoStmt->label + ";");
    }
    else if (auto forStmt = dynamic_cast<ForStmt*>(stmt)) {
        std::string start = generateExpression(forStmt->start.get());
        std::string end = generateExpression(forStmt->end.get());

        writeLine("for (" + forStmt->variable + " = " + start + "; " +
                 forStmt->variable + " <= " + end + "; " +
                 forStmt->variable + "++) {");

        indentLevel++;
        for (auto& bodyStmt : forStmt->body) {
            generateStatement(bodyStmt.get());
        }
        indentLevel--;

        writeLine("}");
    }
}

std::string CodeGenerator::generateExpression(Expression* expr) {
    if (auto num = dynamic_cast<NumberExpr*>(expr)) {
        return std::to_string(num->value);
    }
    else if (auto ident = dynamic_cast<IdentifierExpr*>(expr)) {
        return ident->name;
    }
    else if (auto binary = dynamic_cast<BinaryExpr*>(expr)) {
        std::string left = generateExpression(binary->left.get());
        std::string right = generateExpression(binary->right.get());
        std::string op = translateOperator(binary->op);
        return "(" + left + " " + op + " " + right + ")";
    }
    else if (auto unary = dynamic_cast<UnaryExpr*>(expr)) {
        std::string operand = generateExpression(unary->operand.get());
        std::string op = translateOperator(unary->op);
        return "(" + op + operand + ")";
    }

    return "0";
}
