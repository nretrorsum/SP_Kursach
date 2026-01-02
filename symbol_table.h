#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

using namespace std;

struct Symbol {
    string name;
    string type;      // "int16_t"
    int declaredLine; // Line where variable was declared
    bool initialized; // Whether variable has been assigned a value
};

class SymbolTable {
private:
    unordered_map<string, Symbol> symbols;
    vector<string> errors;

public:
    // Add a new symbol to the table
    bool declare(const string& name, const string& type, int line) {
        if (symbols.find(name) != symbols.end()) {
            errors.push_back("Semantic Error (Line " + to_string(line) +
                "): Variable '" + name + "' is already declared at line " +
                to_string(symbols[name].declaredLine) + ".");
            return false;
        }
        symbols[name] = {name, type, line, false};
        return true;
    }

    // Check if a variable is declared
    bool isDeclared(const string& name) {
        return symbols.find(name) != symbols.end();
    }

    // Check variable usage (must be declared before use)
    bool checkUsage(const string& name, int line) {
        if (!isDeclared(name)) {
            errors.push_back("Semantic Error (Line " + to_string(line) +
                "): Variable '" + name + "' is not declared.");
            return false;
        }
        return true;
    }

    // Mark variable as initialized
    void setInitialized(const string& name) {
        if (isDeclared(name)) {
            symbols[name].initialized = true;
        }
    }

    // Check if variable is initialized before use
    bool isInitialized(const string& name) {
        if (isDeclared(name)) {
            return symbols[name].initialized;
        }
        return false;
    }

    // Warning for uninitialized variable usage
    void checkInitialized(const string& name, int line) {
        if (isDeclared(name) && !symbols[name].initialized) {
            cout << "Warning (Line " << line << "): Variable '" << name
                 << "' may be used before initialization." << endl;
        }
    }

    // Get all errors
    const vector<string>& getErrors() const {
        return errors;
    }

    // Check if there are any errors
    bool hasErrors() const {
        return !errors.empty();
    }

    // Print all errors
    void printErrors() const {
        for (const auto& error : errors) {
            cout << error << endl;
        }
    }

    // Print symbol table (for debugging)
    void printTable() const {
        cout << "\n=== Symbol Table ===" << endl;
        cout << "Name\t\tType\t\tLine\tInitialized" << endl;
        cout << "--------------------------------------------" << endl;
        for (const auto& pair : symbols) {
            const Symbol& sym = pair.second;
            cout << sym.name << "\t\t" << sym.type << "\t\t"
                 << sym.declaredLine << "\t"
                 << (sym.initialized ? "Yes" : "No") << endl;
        }
        cout << "====================" << endl;
    }

    // Get symbol by name
    Symbol* getSymbol(const string& name) {
        if (isDeclared(name)) {
            return &symbols[name];
        }
        return nullptr;
    }

    // Get all symbols (for code generation)
    const unordered_map<string, Symbol>& getAllSymbols() const {
        return symbols;
    }
};

#endif
