#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "codegen.h"

using namespace std;


void logLineTransformation(const string& sourceCode, const vector<Token>& tokens) {
    stringstream ss(sourceCode);
    string lineContent;
    int currentLine = 1;

    cout << "\n======================================================================" << endl;
    cout << "                    VISUALIZATION OF PARSING PROCESS                  " << endl;
    cout << "======================================================================" << endl;

    while (getline(ss, lineContent)) {
        if (lineContent.empty()) {
            currentLine++;
            continue;
        }

        cout << "SOURCE [Line " << setw(2) << currentLine << "]: " << lineContent << endl;
        cout << "   | " << endl;
        cout << "   +--> TOKENS: ";

        bool hasTokens = false;
        for (const auto& token : tokens) {
            if (token.line == currentLine && token.type != END_OF_FILE) {
                cout << "[" << tokenTypeToString(token.type) << ": '" << token.value << "'] ";
                hasTokens = true;
            }
        }

        if (!hasTokens) {
            cout << "(No tokens / Comment / Empty)";
        }

        cout << "\n----------------------------------------------------------------------" << endl;
        currentLine++;
    }
    cout << "======================================================================\n" << endl;
}

int main(int argc, char* argv[]) {
    string filename = "program.z07"; // Default file

    if (argc > 1) {
        filename = argv[1];
    }

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'" << endl;
        return 1;
    }

    cout << "Processing file: " << filename << endl;

    stringstream buffer;
    buffer << file.rdbuf();
    string sourceCode = buffer.str();
    file.close();

    Lexer lexer(sourceCode);
    vector<Token> tokens = lexer.tokenize();

    logLineTransformation(sourceCode, tokens);

    cout << "Starting Syntax Analysis..." << endl;
    Parser parser(tokens);
    parser.parse();

    // Semantic Analysis
    SemanticAnalyzer semantic(tokens);
    if (!semantic.analyze()) {
        return 1;
    }
    semantic.printSymbolTable();

    // Code Generation
    cout << "\nStarting Code Generation..." << endl;
    CodeGenerator codegen(tokens, semantic.getSymbolTable());
    string cCode = codegen.generate();

    // Save generated C code
    codegen.saveToFile("output.c");

    // Display generated code
    cout << "\n=== Generated C Code ===" << endl;
    cout << cCode;
    cout << "========================\n" << endl;

    // Compile generated C code
    cout << "Compiling generated C code..." << endl;
    int compileResult = system("gcc -o output output.c 2>&1");

    if (compileResult == 0) {
        cout << ">>> Compilation Successful! Executable: ./output <<<" << endl;
    } else {
        cout << ">>> Compilation Failed! <<<" << endl;
    }

    return 0;
}