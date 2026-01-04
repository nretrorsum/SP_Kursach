#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "codegen.h"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    file << content;
}

void writeTokens(const std::vector<Token>& tokens, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "TOKEN LIST\n";
    file << "==========\n\n";
    file << "Line:Col\tType\t\t\tValue\n";
    file << "--------\t----\t\t\t-----\n";

    for (const auto& token : tokens) {
        file << token.line << ":" << token.column << "\t\t"
             << tokenTypeToString(token.type) << "\t\t"
             << token.value << "\n";
    }
}

void writeErrors(const std::vector<std::string>& errors,
                 const std::vector<std::string>& warnings,
                 const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "ERROR REPORT\n";
    file << "============\n\n";

    if (errors.empty() && warnings.empty()) {
        file << "No errors or warnings.\n";
        return;
    }

    if (!errors.empty()) {
        file << "ERRORS:\n";
        for (const auto& error : errors) {
            file << "  - " << error << "\n";
        }
        file << "\n";
    }

    if (!warnings.empty()) {
        file << "WARNINGS:\n";
        for (const auto& warning : warnings) {
            file << "  - " << warning << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    std::string inputFile = "program.l16";

    if (argc > 1) {
        inputFile = argv[1];
    }

    std::cout << "L16 Language Translator\n";
    std::cout << "Variant: Laboyko Valeriy Valeriyovych\n";
    std::cout << "==================================\n\n";

    try {
        // Read source file
        std::cout << "Reading source file: " << inputFile << "\n";
        std::string source = readFile(inputFile);

        // Lexical analysis
        std::cout << "Phase 1: Lexical Analysis...\n";
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();
        writeTokens(tokens, "tokens.txt");

        if (!lexer.getErrors().empty()) {
            std::cout << "Lexical errors found:\n";
            for (const auto& error : lexer.getErrors()) {
                std::cout << "  " << error << "\n";
            }
        }

        // Syntactic analysis
        std::cout << "Phase 2: Syntactic Analysis...\n";
        SymbolTable symbolTable;
        Parser parser(tokens, symbolTable);
        auto program = parser.parse();

        if (!parser.getErrors().empty()) {
            std::cout << "Syntax errors found:\n";
            for (const auto& error : parser.getErrors()) {
                std::cout << "  " << error << "\n";
            }
        }

        // Semantic analysis
        std::cout << "Phase 3: Semantic Analysis...\n";
        SemanticAnalyzer semantic(symbolTable);
        semantic.analyze(program.get());

        if (!semantic.getErrors().empty()) {
            std::cout << "Semantic errors found:\n";
            for (const auto& error : semantic.getErrors()) {
                std::cout << "  " << error << "\n";
            }
        }

        if (!semantic.getWarnings().empty()) {
            std::cout << "Warnings:\n";
            for (const auto& warning : semantic.getWarnings()) {
                std::cout << "  " << warning << "\n";
            }
        }

        // Collect all errors
        std::vector<std::string> allErrors;
        std::vector<std::string> allWarnings;

        allErrors.insert(allErrors.end(), lexer.getErrors().begin(), lexer.getErrors().end());
        allErrors.insert(allErrors.end(), parser.getErrors().begin(), parser.getErrors().end());
        allErrors.insert(allErrors.end(), semantic.getErrors().begin(), semantic.getErrors().end());
        allWarnings.insert(allWarnings.end(), semantic.getWarnings().begin(), semantic.getWarnings().end());

        writeErrors(allErrors, allWarnings, "errors.txt");

        if (!allErrors.empty()) {
            std::cout << "\nTranslation failed due to errors.\n";
            return 1;
        }

        // Code generation
        std::cout << "Phase 4: Code Generation...\n";
        CodeGenerator codegen;
        std::string cCode = codegen.generate(program.get());
        writeFile("output.c", cCode);
        std::cout << "Generated C code saved to output.c\n";

        // Compile generated C code
        std::cout << "Compiling generated C code...\n";
        int result = system("gcc -o output output.c -w");

        if (result == 0) {
            std::cout << "\nTranslation successful!\n";
            std::cout << "Run ./output to execute the program.\n";
        } else {
            std::cout << "\nC compilation failed.\n";
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
