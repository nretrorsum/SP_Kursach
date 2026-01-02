#include "semantic.h"

SemanticAnalyzer::SemanticAnalyzer(vector<Token> t) : tokens(t), pos(0) {}

Token SemanticAnalyzer::current() {
    return tokens[pos];
}

Token SemanticAnalyzer::peek(int offset) {
    if (pos + offset < tokens.size()) {
        return tokens[pos + offset];
    }
    return tokens.back();
}

void SemanticAnalyzer::advance() {
    if (pos < tokens.size() - 1) {
        pos++;
    }
}

bool SemanticAnalyzer::match(TokenType type) {
    if (current().type == type) {
        advance();
        return true;
    }
    return false;
}

bool SemanticAnalyzer::analyze() {
    cout << "\nStarting Semantic Analysis..." << endl;
    analyzeProgram();

    if (symbolTable.hasErrors()) {
        symbolTable.printErrors();
        return false;
    }

    cout << ">>> Semantic Analysis Completed Successfully! <<<" << endl;
    return true;
}

void SemanticAnalyzer::analyzeProgram() {
    // program name;
    match(K_PROGRAM);
    match(IDENTIFIER);
    match(SEMICOLON);

    // var int16_t ...
    match(K_VAR);
    analyzeVariableDeclaration();

    // start ... finish
    analyzeCompoundStatement();
}

void SemanticAnalyzer::analyzeVariableDeclaration() {
    match(K_INT16_T);

    // First identifier
    if (current().type == IDENTIFIER) {
        symbolTable.declare(current().value, "int16_t", current().line);
        advance();
    }

    // Additional identifiers
    while (current().type == COMMA) {
        advance(); // skip comma
        if (current().type == IDENTIFIER) {
            symbolTable.declare(current().value, "int16_t", current().line);
            advance();
        }
    }
}

void SemanticAnalyzer::analyzeCompoundStatement() {
    match(K_START);

    while (current().type != K_FINISH && current().type != END_OF_FILE) {
        analyzeStatement();
        if (current().type == SEMICOLON) {
            advance();
        }
    }

    match(K_FINISH);
}

void SemanticAnalyzer::analyzeStatement() {
    TokenType t = current().type;

    if (t == IDENTIFIER) {
        analyzeAssignment();
    } else if (t == K_GET) {
        analyzeInput();
    } else if (t == K_PUT) {
        analyzeOutput();
    } else if (t == K_IF) {
        analyzeIf();
    } else if (t == K_FOR) {
        analyzeFor();
    } else if (t == K_START) {
        analyzeCompoundStatement();
    }
}

void SemanticAnalyzer::analyzeAssignment() {
    // Check if variable is declared
    string varName = current().value;
    int line = current().line;

    symbolTable.checkUsage(varName, line);
    advance(); // skip identifier

    match(OP_ASSIGN); // skip <-

    // Analyze right side of assignment
    analyzeExpression();

    // Mark variable as initialized after assignment
    symbolTable.setInitialized(varName);
}

void SemanticAnalyzer::analyzeInput() {
    match(K_GET);

    // Check if variable is declared
    if (current().type == IDENTIFIER) {
        string varName = current().value;
        int line = current().line;

        symbolTable.checkUsage(varName, line);
        symbolTable.setInitialized(varName); // Input initializes the variable
        advance();
    }
}

void SemanticAnalyzer::analyzeOutput() {
    match(K_PUT);
    analyzeExpression();
}

void SemanticAnalyzer::analyzeIf() {
    match(K_IF);
    analyzeExpression(); // condition
    analyzeCompoundStatement(); // then block

    if (current().type == K_ELSE) {
        advance();
        analyzeCompoundStatement(); // else block
    }
}

void SemanticAnalyzer::analyzeFor() {
    match(K_FOR);

    // Loop variable
    if (current().type == IDENTIFIER) {
        string varName = current().value;
        int line = current().line;

        symbolTable.checkUsage(varName, line);
        symbolTable.setInitialized(varName); // For loop initializes the variable
        advance();
    }

    match(K_IN);
    analyzeExpression(); // start value
    match(RANGE);
    analyzeExpression(); // end value
    analyzeCompoundStatement(); // loop body
}

void SemanticAnalyzer::analyzeExpression() {
    analyzeExpressionRecursive();
}

void SemanticAnalyzer::analyzeExpressionRecursive() {
    // Process current token
    TokenType t = current().type;

    if (t == IDENTIFIER) {
        // Check if variable is declared and warn if not initialized
        string varName = current().value;
        int line = current().line;

        symbolTable.checkUsage(varName, line);
        symbolTable.checkInitialized(varName, line);
        advance();
    } else if (t == NUMBER) {
        advance();
    } else if (t == LPAREN) {
        advance(); // skip (
        analyzeExpressionRecursive();
        match(RPAREN); // skip )
    } else if (t == OP_NOT) {
        advance();
        analyzeExpressionRecursive();
    }

    // Check for operators and continue parsing
    while (current().type == OP_PLUS || current().type == OP_MINUS ||
           current().type == OP_MUL || current().type == OP_DIV ||
           current().type == OP_MOD || current().type == OP_AND ||
           current().type == OP_OR || current().type == OP_EG ||
           current().type == OP_NE || current().type == OP_GE ||
           current().type == OP_LE) {
        advance(); // skip operator
        analyzeExpressionRecursive();
    }
}

void SemanticAnalyzer::printErrors() {
    symbolTable.printErrors();
}

void SemanticAnalyzer::printSymbolTable() {
    symbolTable.printTable();
}

SymbolTable& SemanticAnalyzer::getSymbolTable() {
    return symbolTable;
}

bool SemanticAnalyzer::hasErrors() {
    return symbolTable.hasErrors();
}
