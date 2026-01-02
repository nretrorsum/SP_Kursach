/**
 * Курсовий проект з Системного Програмування
 * Тема: Розробка транслятора з вхідної мови програмування V07
 * Варіант: Войтович Олександр Вікторович
 *
 * Файл: main.cpp
 * Опис: Головний файл транслятора - точка входу програми
 *
 * Дата створення: 2024
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

// Функція для читання файлу
string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Помилка: Не вдалося відкрити файл '" << filename << "'" << endl;
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Функція для запису файлу
bool writeFile(const string& filename, const string& content) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Помилка: Не вдалося створити файл '" << filename << "'" << endl;
        return false;
    }

    file << content;
    return true;
}

// Візуалізація процесу парсингу (рядок за рядком)
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

// Виведення токенів
void printTokens(const vector<Token>& tokens) {
    cout << "\n========== ЛЕКСИЧНИЙ АНАЛІЗ ==========\n";
    cout << "Список токенів:\n";
    cout << "-------------------------------------------\n";

    int currentLine = 0;
    for (const auto& token : tokens) {
        if (token.line != currentLine) {
            if (currentLine != 0) cout << "\n";
            cout << "Рядок " << token.line << ": ";
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

// Виведення таблиці символів
void printSymbolTable(SymbolTable& symbolTable) {
    cout << "\n========== ТАБЛИЦЯ СИМВОЛІВ ==========\n";

    cout << "\nЗмінні:\n";
    cout << "-------------------------------------------\n";
    cout << "Ім'я\tТип\tРядок\tІніціалізована\n";
    cout << "-------------------------------------------\n";

    for (const auto& varName : symbolTable.getVariableOrder()) {
        auto var = symbolTable.getVariable(varName);
        if (var) {
            cout << var->name << "\t"
                      << var->dataType << "\t"
                      << var->declarationLine << "\t"
                      << (var->isInitialized ? "Так" : "Ні") << "\n";
        }
    }

    if (!symbolTable.getLabelOrder().empty()) {
        cout << "\nМітки:\n";
        cout << "-------------------------------------------\n";
        cout << "Ім'я\tРядок\tВизначена\n";
        cout << "-------------------------------------------\n";

        for (const auto& labelName : symbolTable.getLabelOrder()) {
            auto label = symbolTable.getLabel(labelName);
            if (label) {
                cout << label->name << "\t"
                          << label->declarationLine << "\t"
                          << (label->isDefined ? "Так" : "Ні") << "\n";
            }
        }
    }
    cout << "-------------------------------------------\n";
}

// Виведення помилок
void printErrors(const string& phase, const vector<string>& errors) {
    if (errors.empty()) return;

    cout << "\n========== ПОМИЛКИ (" << phase << ") ==========\n";
    for (const auto& error : errors) {
        cout << "  " << error << "\n";
    }
}

// Виведення попереджень
void printWarnings(const vector<string>& warnings) {
    if (warnings.empty()) return;

    cout << "\n========== ПОПЕРЕДЖЕННЯ ==========\n";
    for (const auto& warning : warnings) {
        cout << "  " << warning << "\n";
    }
}

int main(int argc, char* argv[]) {
    cout << "=============================================\n";
    cout << "  Транслятор мови V07\n";
    cout << "  Варіант: Войтович Олександр Вікторович\n";
    cout << "  Курсовий проект з Системного Програмування\n";
    cout << "=============================================\n";

    // Визначення вхідного файлу
    string inputFile = "program.v07";
    if (argc > 1) {
        inputFile = argv[1];
    }

    // Читання вхідного файлу
    cout << "\nВхідний файл: " << inputFile << "\n";
    string source = readFile(inputFile);
    if (source.empty()) {
        return 1;
    }

    cout << "\n========== ВХІДНИЙ КОД ==========\n";
    cout << source << "\n";

    bool hasErrors = false;

    // 1. ЛЕКСИЧНИЙ АНАЛІЗ
    cout << "\n[1] Виконується лексичний аналіз...\n";
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    // Візуалізація процесу парсингу
    logLineTransformation(source, tokens);

    printTokens(tokens);

    if (lexer.hasErrors()) {
        printErrors("Лексичний аналіз", lexer.getErrors());
        hasErrors = true;
    }

    // Збереження токенів у файл
    ofstream tokensFile("tokens.txt");
    if (tokensFile.is_open()) {
        for (const auto& token : tokens) {
            tokensFile << "Рядок " << token.line << ", Стовпець " << token.column
                       << ": " << tokenTypeToString(token.type)
                       << " = \"" << token.value << "\"\n";
        }
        tokensFile.close();
        cout << "Токени збережено у файл: tokens.txt\n";
    }

    // 2. СИНТАКСИЧНИЙ АНАЛІЗ
    cout << "\n[2] Виконується синтаксичний аналіз...\n";
    Parser parser(tokens);
    auto program = parser.parse();

    if (parser.hasErrors()) {
        printErrors("Синтаксичний аналіз", parser.getErrors());
        hasErrors = true;
    } else {
        cout << "Синтаксичний аналіз завершено успішно.\n";
        cout << "Ім'я програми: " << program->name << "\n";
        cout << "Кількість змінних: " << program->variables.size() << "\n";
        cout << "Кількість операторів: " << program->statements.size() << "\n";
    }

    // 3. СЕМАНТИЧНИЙ АНАЛІЗ
    cout << "\n[3] Виконується семантичний аналіз...\n";
    SemanticAnalyzer semantic(parser.getSymbolTable());
    semantic.analyze(*program);

    printSymbolTable(parser.getSymbolTable());

    if (semantic.hasErrors()) {
        printErrors("Семантичний аналіз", semantic.getErrors());
        hasErrors = true;
    } else {
        cout << "Семантичний аналіз завершено успішно.\n";
    }

    if (semantic.hasWarnings()) {
        printWarnings(semantic.getWarnings());
    }

    // Збереження помилок у файл
    ofstream errorsFile("errors.txt");
    if (errorsFile.is_open()) {
        if (!hasErrors && !semantic.hasWarnings()) {
            errorsFile << "Помилок не виявлено.\n";
        } else {
            if (lexer.hasErrors()) {
                errorsFile << "=== Лексичні помилки ===\n";
                for (const auto& err : lexer.getErrors()) {
                    errorsFile << err << "\n";
                }
            }
            if (parser.hasErrors()) {
                errorsFile << "=== Синтаксичні помилки ===\n";
                for (const auto& err : parser.getErrors()) {
                    errorsFile << err << "\n";
                }
            }
            if (semantic.hasErrors()) {
                errorsFile << "=== Семантичні помилки ===\n";
                for (const auto& err : semantic.getErrors()) {
                    errorsFile << err << "\n";
                }
            }
            if (semantic.hasWarnings()) {
                errorsFile << "=== Попередження ===\n";
                for (const auto& warn : semantic.getWarnings()) {
                    errorsFile << warn << "\n";
                }
            }
        }
        errorsFile.close();
        cout << "Звіт про помилки збережено у файл: errors.txt\n";
    }

    // 4. ГЕНЕРАЦІЯ КОДУ
    if (hasErrors) {
        cout << "\n[!] Генерація коду пропущена через наявність помилок.\n";
        return 1;
    }

    cout << "\n[4] Виконується генерація коду...\n";
    CodeGenerator codegen;
    string cCode = codegen.generate(*program);

    cout << "\n========== ЗГЕНЕРОВАНИЙ C КОД ==========\n";
    cout << cCode;
    cout << "=========================================\n";

    // Збереження C коду
    string outputCFile = "output.c";
    if (writeFile(outputCFile, cCode)) {
        cout << "C код збережено у файл: " << outputCFile << "\n";
    }

    // 5. КОМПІЛЯЦІЯ C КОДУ
    cout << "\n[5] Компіляція згенерованого C коду...\n";
    string compileCommand = "gcc -o output " + outputCFile + " 2>&1";
    int compileResult = system(compileCommand.c_str());

    if (compileResult == 0) {
        cout << "Компіляція успішна! Виконуваний файл: output\n";
        cout << "\n=============================================\n";
        cout << "  Трансляція завершена успішно!\n";
        cout << "=============================================\n";
    } else {
        cout << "Помилка компіляції C коду.\n";
        return 1;
    }

    return 0;
}
