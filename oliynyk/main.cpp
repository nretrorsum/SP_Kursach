/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
 *
 * File: main.cpp
 * Description: Main translator file - program entry point
 *
 * Created: 2024
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>

#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "codegen.h"

using namespace std;

// Function to read a file
string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'" << endl;
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to write a file
bool writeFile(const string& filename, const string& content) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not create file '" << filename << "'" << endl;
        return false;
    }

    file << content;
    return true;
}

// Visualization of parsing process (line by line)
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
            if (token.line == currentLine && token.type != TokenType::END_OF_FILE) {
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

// Print tokens
void printTokens(const vector<Token>& tokens) {
    cout << "\n========== LEXICAL ANALYSIS ==========" << endl;
    cout << "Token list:" << endl;
    cout << "-------------------------------------------" << endl;

    int currentLine = 0;
    for (const auto& token : tokens) {
        if (token.line != currentLine) {
            if (currentLine != 0) cout << "\n";
            cout << "Line " << token.line << ": ";
            currentLine = token.line;
        }

        cout << "[" << tokenTypeToString(token.type);
        if (!token.value.empty()) {
            cout << ": " << token.value;
        }
        cout << "] ";
    }
    cout << "\n-------------------------------------------" << endl;
}

// Print symbol table
void printSymbolTable(SymbolTable& symbolTable) {
    cout << "\n========== SYMBOL TABLE ==========" << endl;

    cout << "\nVariables:" << endl;
    cout << "-------------------------------------------" << endl;
    cout << "Name\tType\tLine\tInitialized" << endl;
    cout << "-------------------------------------------" << endl;

    for (const auto& varName : symbolTable.getVariableOrder()) {
        auto var = symbolTable.getVariable(varName);
        if (var) {
            cout << var->name << "\t"
                      << var->dataType << "\t"
                      << var->declarationLine << "\t"
                      << (var->isInitialized ? "Yes" : "No") << "\n";
        }
    }
    cout << "-------------------------------------------" << endl;
}

// Print errors
void printErrors(const string& phase, const vector<string>& errors) {
    if (errors.empty()) return;

    cout << "\n========== ERRORS (" << phase << ") ==========" << endl;
    for (const auto& error : errors) {
        cout << "  " << error << "\n";
    }
}

// Print warnings
void printWarnings(const vector<string>& warnings) {
    if (warnings.empty()) return;

    cout << "\n========== WARNINGS ==========" << endl;
    for (const auto& warning : warnings) {
        cout << "  " << warning << "\n";
    }
}

int main(int argc, char* argv[]) {
    cout << "=============================================" << endl;
    cout << "  O16 Language Translator" << endl;
    cout << "  Variant: Oliynyk Bohdan Romanovych" << endl;
    cout << "  Course Project on System Programming" << endl;
    cout << "=============================================" << endl;

    // Determine input file
    string inputFile = "program.o16";
    if (argc > 1) {
        inputFile = argv[1];
    }

    // Read input file
    cout << "\nInput file: " << inputFile << "\n";
    string source = readFile(inputFile);
    if (source.empty()) {
        return 1;
    }

    cout << "\n========== SOURCE CODE ==========" << endl;
    cout << source << "\n";

    bool hasErrors = false;

    // 1. LEXICAL ANALYSIS
    cout << "\n[1] Performing lexical analysis..." << endl;
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    // Visualization of parsing process
    logLineTransformation(source, tokens);

    printTokens(tokens);

    if (lexer.hasErrors()) {
        printErrors("Lexical Analysis", lexer.getErrors());
        hasErrors = true;
    }

    // Save tokens to file
    ofstream tokensFile("tokens.txt");
    if (tokensFile.is_open()) {
        for (const auto& token : tokens) {
            tokensFile << "Line " << token.line << ", Column " << token.column
                       << ": " << tokenTypeToString(token.type)
                       << " = \"" << token.value << "\"\n";
        }
        tokensFile.close();
        cout << "Tokens saved to file: tokens.txt" << endl;
    }

    // 2. SYNTAX ANALYSIS
    cout << "\n[2] Performing syntax analysis..." << endl;
    Parser parser(tokens);
    auto program = parser.parse();

    if (parser.hasErrors()) {
        printErrors("Syntax Analysis", parser.getErrors());
        hasErrors = true;
    } else {
        cout << "Syntax analysis completed successfully." << endl;
        cout << "Number of variables: " << program->variables.size() << "\n";
        cout << "Number of statements: " << program->statements.size() << "\n";
    }

    // 3. SEMANTIC ANALYSIS
    cout << "\n[3] Performing semantic analysis..." << endl;
    SemanticAnalyzer semantic(parser.getSymbolTable());
    semantic.analyze(*program);

    printSymbolTable(parser.getSymbolTable());

    if (semantic.hasErrors()) {
        printErrors("Semantic Analysis", semantic.getErrors());
        hasErrors = true;
    } else {
        cout << "Semantic analysis completed successfully." << endl;
    }

    if (semantic.hasWarnings()) {
        printWarnings(semantic.getWarnings());
    }

    // Save errors to file
    ofstream errorsFile("errors.txt");
    if (errorsFile.is_open()) {
        if (!hasErrors && !semantic.hasWarnings()) {
            errorsFile << "No errors detected.\n";
        } else {
            if (lexer.hasErrors()) {
                errorsFile << "=== Lexical Errors ===\n";
                for (const auto& err : lexer.getErrors()) {
                    errorsFile << err << "\n";
                }
            }
            if (parser.hasErrors()) {
                errorsFile << "=== Syntax Errors ===\n";
                for (const auto& err : parser.getErrors()) {
                    errorsFile << err << "\n";
                }
            }
            if (semantic.hasErrors()) {
                errorsFile << "=== Semantic Errors ===\n";
                for (const auto& err : semantic.getErrors()) {
                    errorsFile << err << "\n";
                }
            }
            if (semantic.hasWarnings()) {
                errorsFile << "=== Warnings ===\n";
                for (const auto& warn : semantic.getWarnings()) {
                    errorsFile << warn << "\n";
                }
            }
        }
        errorsFile.close();
        cout << "Error report saved to file: errors.txt" << endl;
    }

    // 4. CODE GENERATION
    if (hasErrors) {
        cout << "\n[!] Code generation skipped due to errors." << endl;
        return 1;
    }

    cout << "\n[4] Performing code generation..." << endl;
    CodeGenerator codegen;
    string cCode = codegen.generate(*program);

    cout << "\n========== GENERATED C CODE ==========" << endl;
    cout << cCode;
    cout << "=======================================" << endl;

    // Save C code
    string outputCFile = "output.c";
    if (writeFile(outputCFile, cCode)) {
        cout << "C code saved to file: " << outputCFile << "\n";
    }

    // 5. C CODE COMPILATION
    cout << "\n[5] Compiling generated C code..." << endl;
    string compileCommand = "gcc -o output " + outputCFile + " 2>&1";
    int compileResult = system(compileCommand.c_str());

    if (compileResult == 0) {
        cout << "Compilation successful! Executable: output" << endl;
        cout << "\n=============================================" << endl;
        cout << "  Translation completed successfully!" << endl;
        cout << "=============================================" << endl;
    } else {
        cout << "C code compilation error." << endl;
        return 1;
    }

    return 0;
}
