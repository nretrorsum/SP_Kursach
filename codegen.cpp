#include "codegen.h"
#include <fstream>

CodeGenerator::CodeGenerator(vector<Token> t, SymbolTable& st)
    : tokens(t), pos(0), symbolTable(st), indentLevel(0) {}

Token CodeGenerator::current() {
    return tokens[pos];
}

Token CodeGenerator::peek(int offset) {
    if (pos + offset < tokens.size()) {
        return tokens[pos + offset];
    }
    return tokens.back();
}

void CodeGenerator::advance() {
    if (pos < tokens.size() - 1) {
        pos++;
    }
}

bool CodeGenerator::match(TokenType type) {
    if (current().type == type) {
        advance();
        return true;
    }
    return false;
}

void CodeGenerator::indent() {
    for (int i = 0; i < indentLevel; i++) {
        output << "    ";
    }
}

void CodeGenerator::emit(const string& code) {
    output << code;
}

void CodeGenerator::emitLine(const string& code) {
    indent();
    output << code << "\n";
}

string CodeGenerator::generate() {
    // Header
    output << "/* Generated C code from Z07 language */\n";
    output << "/* Translator by Zaplyvanyi Daniil */\n\n";
    output << "#include <stdio.h>\n";
    output << "#include <stdint.h>\n\n";

    generateProgram();

    return output.str();
}

void CodeGenerator::generateProgram() {
    // program name;
    match(K_PROGRAM);
    string programName = current().value;
    advance(); // skip name
    match(SEMICOLON);

    // var int16_t ...
    match(K_VAR);

    output << "int main() {\n";
    indentLevel++;

    generateVariableDeclaration();

    output << "\n";

    // start ... finish
    generateCompoundStatement();

    output << "\n";
    emitLine("return 0;");

    indentLevel--;
    output << "}\n";
}

void CodeGenerator::generateVariableDeclaration() {
    match(K_INT16_T);

    // Collect all variable names
    vector<string> varNames;

    if (current().type == IDENTIFIER) {
        varNames.push_back(current().value);
        advance();
    }

    while (current().type == COMMA) {
        advance(); // skip comma
        if (current().type == IDENTIFIER) {
            varNames.push_back(current().value);
            advance();
        }
    }

    // Emit declaration
    indent();
    output << "int16_t ";
    for (size_t i = 0; i < varNames.size(); i++) {
        output << varNames[i];
        if (i < varNames.size() - 1) {
            output << ", ";
        }
    }
    output << ";\n";
}

void CodeGenerator::generateCompoundStatement() {
    match(K_START);

    while (current().type != K_FINISH && current().type != END_OF_FILE) {
        generateStatement();
        if (current().type == SEMICOLON) {
            advance();
        }
    }

    match(K_FINISH);
}

void CodeGenerator::generateStatement() {
    TokenType t = current().type;

    if (t == IDENTIFIER) {
        generateAssignment();
    } else if (t == K_GET) {
        generateInput();
    } else if (t == K_PUT) {
        generateOutput();
    } else if (t == K_IF) {
        generateIf();
    } else if (t == K_FOR) {
        generateFor();
    } else if (t == K_START) {
        emitLine("{");
        indentLevel++;
        generateCompoundStatement();
        indentLevel--;
        emitLine("}");
    }
}

void CodeGenerator::generateAssignment() {
    string varName = current().value;
    advance(); // skip identifier
    match(OP_ASSIGN); // skip <-

    string expr = generateExpression();

    emitLine(varName + " = " + expr + ";");
}

void CodeGenerator::generateInput() {
    match(K_GET);

    string varName = current().value;
    advance();

    emitLine("scanf(\"%hd\", &" + varName + ");");
}

void CodeGenerator::generateOutput() {
    match(K_PUT);

    string expr = generateExpression();

    emitLine("printf(\"%d\\n\", " + expr + ");");
}

void CodeGenerator::generateIf() {
    match(K_IF);

    string condition = generateExpression();

    emitLine("if (" + condition + ") {");
    indentLevel++;
    generateCompoundStatement();
    indentLevel--;

    if (current().type == K_ELSE) {
        advance();
        emitLine("} else {");
        indentLevel++;
        generateCompoundStatement();
        indentLevel--;
    }

    emitLine("}");
}

void CodeGenerator::generateFor() {
    match(K_FOR);

    string loopVar = current().value;
    advance(); // skip identifier

    match(K_IN);

    string startExpr = generateExpression();
    match(RANGE);
    string endExpr = generateExpression();

    emitLine("for (" + loopVar + " = " + startExpr + "; " +
             loopVar + " <= " + endExpr + "; " + loopVar + "++) {");
    indentLevel++;
    generateForBody();
    indentLevel--;
    emitLine("}");
}

void CodeGenerator::generateForBody() {
    match(LBRACE);

    while (current().type != RBRACE && current().type != END_OF_FILE) {
        generateStatement();
        if (current().type == SEMICOLON) {
            advance();
        }
    }

    match(RBRACE);
}

string CodeGenerator::generateExpression() {
    stringstream expr;

    // Process expression tokens until we hit a statement terminator
    while (current().type != SEMICOLON &&
           current().type != K_START &&
           current().type != K_FINISH &&
           current().type != K_ELSE &&
           current().type != RANGE &&
           current().type != LBRACE &&
           current().type != END_OF_FILE) {

        TokenType t = current().type;

        if (t == IDENTIFIER || t == NUMBER) {
            expr << current().value;
            advance();
        } else if (t == OP_PLUS) {
            expr << " + ";
            advance();
        } else if (t == OP_MINUS) {
            expr << " - ";
            advance();
        } else if (t == OP_MUL) {
            expr << " * ";
            advance();
        } else if (t == OP_DIV) {
            expr << " / ";
            advance();
        } else if (t == OP_MOD) {
            expr << " % ";
            advance();
        } else if (t == OP_EG) {
            expr << " == ";
            advance();
        } else if (t == OP_NE) {
            expr << " != ";
            advance();
        } else if (t == OP_GE) {
            expr << " >= ";
            advance();
        } else if (t == OP_LE) {
            expr << " <= ";
            advance();
        } else if (t == OP_AND) {
            expr << " && ";
            advance();
        } else if (t == OP_OR) {
            expr << " || ";
            advance();
        } else if (t == OP_NOT) {
            expr << "(!";
            advance();
            // Get the next operand and close parenthesis
            if (current().type == IDENTIFIER || current().type == NUMBER) {
                expr << current().value << ")";
                advance();
                continue;
            } else if (current().type == LPAREN) {
                // Handle !(expr)
                expr << "(";
                advance();
                int parenCount = 1;
                while (parenCount > 0 && current().type != END_OF_FILE) {
                    if (current().type == LPAREN) parenCount++;
                    if (current().type == RPAREN) parenCount--;
                    if (current().type == IDENTIFIER || current().type == NUMBER) {
                        expr << current().value;
                    } else if (current().type == OP_PLUS) expr << " + ";
                    else if (current().type == OP_MINUS) expr << " - ";
                    else if (current().type == OP_MUL) expr << " * ";
                    else if (current().type == OP_DIV) expr << " / ";
                    else if (current().type == OP_MOD) expr << " % ";
                    else if (current().type == OP_GE) expr << " >= ";
                    else if (current().type == OP_LE) expr << " <= ";
                    else if (current().type == OP_EG) expr << " == ";
                    else if (current().type == OP_NE) expr << " != ";
                    else if (current().type == OP_AND) expr << " && ";
                    else if (current().type == OP_OR) expr << " || ";
                    else if (current().type == LPAREN) expr << "(";
                    else if (current().type == RPAREN) expr << ")";
                    advance();
                }
                expr << ")";
                continue;
            }
        } else if (t == LPAREN) {
            expr << "(";
            advance();
        } else if (t == RPAREN) {
            expr << ")";
            advance();
        } else {
            break;
        }
    }

    return expr.str();
}

bool CodeGenerator::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not create file '" << filename << "'" << endl;
        return false;
    }

    file << output.str();
    file.close();

    cout << "Generated C code saved to: " << filename << endl;
    return true;
}
