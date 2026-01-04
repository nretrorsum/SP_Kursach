/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
 *
 * File: symbol_table.h
 * Description: Symbol table for storing information about variables
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>

// Symbol type
enum class SymbolType {
    VARIABLE
};

// Symbol information
struct SymbolInfo {
    std::string name;
    SymbolType type;
    std::string dataType;  // int_2
    int declarationLine;
    bool isInitialized;

    SymbolInfo(const std::string& n = "", SymbolType t = SymbolType::VARIABLE,
               const std::string& dt = "int_2", int line = 0)
        : name(n), type(t), dataType(dt), declarationLine(line),
          isInitialized(false) {}
};

// Symbol table
class SymbolTable {
public:
    // Add variable
    bool addVariable(const std::string& name, int line) {
        if (variables.find(name) != variables.end()) {
            return false; // Variable already exists
        }
        variables[name] = SymbolInfo(name, SymbolType::VARIABLE, "int_2", line);
        variableOrder.push_back(name);
        return true;
    }

    // Check if variable exists
    bool hasVariable(const std::string& name) const {
        return variables.find(name) != variables.end();
    }

    // Get variable information
    SymbolInfo* getVariable(const std::string& name) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    // Mark variable as initialized
    void setInitialized(const std::string& name) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            it->second.isInitialized = true;
        }
    }

    // Check if variable is initialized
    bool isInitialized(const std::string& name) const {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return it->second.isInitialized;
        }
        return false;
    }

    // Get all variables (in declaration order)
    const std::vector<std::string>& getVariableOrder() const {
        return variableOrder;
    }

    // Get variables map
    const std::unordered_map<std::string, SymbolInfo>& getVariables() const {
        return variables;
    }

    // Clear table
    void clear() {
        variables.clear();
        variableOrder.clear();
    }

private:
    std::unordered_map<std::string, SymbolInfo> variables;
    std::vector<std::string> variableOrder;
};

#endif // SYMBOL_TABLE_H
