/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V03 programming language
 * Variant: Voitovych Oleksandr Viktorovych
 *
 * File: symbol_table.h
 * Description: Symbol table for storing information about variables and labels
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>

// Symbol type
enum class SymbolType {
    VARIABLE,
    LABEL
};

// Symbol information
struct SymbolInfo {
    std::string name;
    SymbolType type;
    std::string dataType;  // Int16
    int declarationLine;
    bool isInitialized;
    bool isDefined;        // For labels - whether label is defined

    SymbolInfo(const std::string& n = "", SymbolType t = SymbolType::VARIABLE,
               const std::string& dt = "Int16", int line = 0)
        : name(n), type(t), dataType(dt), declarationLine(line),
          isInitialized(false), isDefined(false) {}
};

// Symbol table
class SymbolTable {
public:
    // Add variable
    bool addVariable(const std::string& name, int line) {
        if (variables.find(name) != variables.end()) {
            return false; // Variable already exists
        }
        variables[name] = SymbolInfo(name, SymbolType::VARIABLE, "Int16", line);
        variableOrder.push_back(name);
        return true;
    }

    // Add label
    bool addLabel(const std::string& name, int line) {
        if (labels.find(name) != labels.end()) {
            // Label already used, mark as defined
            labels[name].isDefined = true;
            return true;
        }
        SymbolInfo info(name, SymbolType::LABEL, "", line);
        info.isDefined = true;
        labels[name] = info;
        labelOrder.push_back(name);
        return true;
    }

    // Register label usage (for Goto)
    void useLabel(const std::string& name, int line) {
        if (labels.find(name) == labels.end()) {
            SymbolInfo info(name, SymbolType::LABEL, "", line);
            info.isDefined = false;
            labels[name] = info;
            labelOrder.push_back(name);
        }
    }

    // Check if variable exists
    bool hasVariable(const std::string& name) const {
        return variables.find(name) != variables.end();
    }

    // Check if label exists
    bool hasLabel(const std::string& name) const {
        return labels.find(name) != labels.end();
    }

    // Check if label is defined
    bool isLabelDefined(const std::string& name) const {
        auto it = labels.find(name);
        if (it != labels.end()) {
            return it->second.isDefined;
        }
        return false;
    }

    // Get variable information
    SymbolInfo* getVariable(const std::string& name) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    // Get label information
    SymbolInfo* getLabel(const std::string& name) {
        auto it = labels.find(name);
        if (it != labels.end()) {
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

    // Get all labels
    const std::vector<std::string>& getLabelOrder() const {
        return labelOrder;
    }

    // Get all undefined labels
    std::vector<std::string> getUndefinedLabels() const {
        std::vector<std::string> undefined;
        for (const auto& pair : labels) {
            if (!pair.second.isDefined) {
                undefined.push_back(pair.first);
            }
        }
        return undefined;
    }

    // Get variables map
    const std::unordered_map<std::string, SymbolInfo>& getVariables() const {
        return variables;
    }

    // Get labels map
    const std::unordered_map<std::string, SymbolInfo>& getLabels() const {
        return labels;
    }

    // Clear table
    void clear() {
        variables.clear();
        labels.clear();
        variableOrder.clear();
        labelOrder.clear();
    }

private:
    std::unordered_map<std::string, SymbolInfo> variables;
    std::unordered_map<std::string, SymbolInfo> labels;
    std::vector<std::string> variableOrder;
    std::vector<std::string> labelOrder;
};

#endif // SYMBOL_TABLE_H
