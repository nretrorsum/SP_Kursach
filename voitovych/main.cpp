/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V03 programming language
 * Variant: Voitovych Oleksandr Viktorovych
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
    cout << "\n========== LEXICAL ANALYSIS ==========\n";
    cout << "Token list:\n";
    cout << "-------------------------------------------\n";

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
    cout << "\n-------------------------------------------\n";
}

// Print symbol table
void printSymbolTable(SymbolTable& symbolTable) {
    cout << "\n========== SYMBOL TABLE ==========\n";

    cout << "\nVariables:\n";
    cout << "-------------------------------------------\n";
    cout << "Name\tType\tLine\tInitialized\n";
    cout << "-------------------------------------------\n";

    for (const auto& varName : symbolTable.getVariableOrder()) {
        auto var = symbolTable.getVariable(varName);
        if (var) {
            cout << var->name << "\t"
                      << var->dataType << "\t"
                      << var->declarationLine << "\t"
                      << (var->isInitialized ? "Yes" : "No") << "\n";
        }
    }

    if (!symbolTable.getLabelOrder().empty()) {
        cout << "\nLabels:\n";
        cout << "-------------------------------------------\n";
        cout << "Name\tLine\tDefined\n";
        cout << "-------------------------------------------\n";

        for (const auto& labelName : symbolTable.getLabelOrder()) {
            auto label = symbolTable.getLabel(labelName);
            if (label) {
                cout << label->name << "\t"
                          << label->declarationLine << "\t"
                          << (label->isDefined ? "Yes" : "No") << "\n";
            }
        }
    }
    cout << "-------------------------------------------\n";
}

// Print errors
void printErrors(const string& phase, const vector<string>& errors) {
    if (errors.empty()) return;

    cout << "\n========== ERRORS (" << phase << ") ==========\n";
    for (const auto& error : errors) {
        cout << "  " << error << "\n";
    }
}

// Print warnings
void printWarnings(const vector<string>& warnings) {
    if (warnings.empty()) return;

    cout << "\n========== WARNINGS ==========\n";
    for (const auto& warning : warnings) {
        cout << "  " << warning << "\n";
    }
}

int main(int argc, char* argv[]) {
    cout << "=============================================\n";
    cout << "  V03 Language Translator\n";
    cout << "  Variant: Voitovych Oleksandr Viktorovych\n";
    cout << "  Course Project on System Programming\n";
    cout << "=============================================\n";

    // Determine input file
    string inputFile = "program.v03";
    if (argc > 1) {
        inputFile = argv[1];
    }

    // Read input file
    cout << "\nInput file: " << inputFile << "\n";
    string source = readFile(inputFile);
    if (source.empty()) {
        return 1;
    }

    cout << "\n========== SOURCE CODE ==========\n";
    cout << source << "\n";

    bool hasErrors = false;

    // 1. LEXICAL ANALYSIS
    cout << "\n[1] Performing lexical analysis...\n";
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
        cout << "Tokens saved to file: tokens.txt\n";
    }

    // 2. SYNTAX ANALYSIS
    cout << "\n[2] Performing syntax analysis...\n";
    Parser parser(tokens);
    auto program = parser.parse();

    if (parser.hasErrors()) {
        printErrors("Syntax Analysis", parser.getErrors());
        hasErrors = true;
    } else {
        cout << "Syntax analysis completed successfully.\n";
        cout << "Program name: " << program->name << "\n";
        cout << "Number of variables: " << program->variables.size() << "\n";
        cout << "Number of statements: " << program->statements.size() << "\n";
    }

    // 3. SEMANTIC ANALYSIS
    cout << "\n[3] Performing semantic analysis...\n";
    SemanticAnalyzer semantic(parser.getSymbolTable());
    semantic.analyze(*program);

    printSymbolTable(parser.getSymbolTable());

    if (semantic.hasErrors()) {
        printErrors("Semantic Analysis", semantic.getErrors());
        hasErrors = true;
    } else {
        cout << "Semantic analysis completed successfully.\n";
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
        cout << "Error report saved to file: errors.txt\n";
    }

    // 4. CODE GENERATION
    if (hasErrors) {
        cout << "\n[!] Code generation skipped due to errors.\n";
        return 1;
    }

    cout << "\n[4] Performing code generation...\n";
    CodeGenerator codegen;
    string cCode = codegen.generate(*program);

    cout << "\n========== GENERATED C CODE ==========\n";
    cout << cCode;
    cout << "=======================================\n";

    // Save C code
    string outputCFile = "output.c";
    if (writeFile(outputCFile, cCode)) {
        cout << "C code saved to file: " << outputCFile << "\n";
    }

    // 5. C CODE COMPILATION
    cout << "\n[5] Compiling generated C code...\n";
    string compileCommand = "gcc -o output " + outputCFile + " 2>&1";
    int compileResult = system(compileCommand.c_str());

    if (compileResult == 0) {
        cout << "Compilation successful! Executable: output\n";
        cout << "\n=============================================\n";
        cout << "  Translation completed successfully!\n";
        cout << "=============================================\n";
    } else {
        cout << "C code compilation error.\n";
        return 1;
    }

    return 0;
}
