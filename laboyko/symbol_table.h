#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>

struct Symbol {
    std::string name;
    std::string type;
    bool initialized;
    int line;

    Symbol(const std::string& n = "", const std::string& t = "Integer32_t", int l = 0)
        : name(n), type(t), initialized(false), line(l) {}
};

class SymbolTable {
private:
    std::unordered_map<std::string, Symbol> symbols;
    std::unordered_map<std::string, int> labels;  // Label name -> line number

public:
    bool declare(const std::string& name, int line) {
        if (symbols.find(name) != symbols.end()) {
            return false;  // Already declared
        }
        symbols[name] = Symbol(name, "Integer32_t", line);
        return true;
    }

    bool isDeclared(const std::string& name) const {
        return symbols.find(name) != symbols.end();
    }

    Symbol* get(const std::string& name) {
        auto it = symbols.find(name);
        if (it != symbols.end()) {
            return &it->second;
        }
        return nullptr;
    }

    void setInitialized(const std::string& name) {
        auto it = symbols.find(name);
        if (it != symbols.end()) {
            it->second.initialized = true;
        }
    }

    bool isInitialized(const std::string& name) const {
        auto it = symbols.find(name);
        if (it != symbols.end()) {
            return it->second.initialized;
        }
        return false;
    }

    // Label management for Basic-style Goto
    bool declareLabel(const std::string& name, int line) {
        if (labels.find(name) != labels.end()) {
            return false;  // Already declared
        }
        labels[name] = line;
        return true;
    }

    bool isLabelDeclared(const std::string& name) const {
        return labels.find(name) != labels.end();
    }

    int getLabelLine(const std::string& name) const {
        auto it = labels.find(name);
        if (it != labels.end()) {
            return it->second;
        }
        return -1;
    }

    std::vector<std::string> getAllSymbols() const {
        std::vector<std::string> result;
        for (const auto& pair : symbols) {
            result.push_back(pair.first);
        }
        return result;
    }

    std::vector<std::string> getAllLabels() const {
        std::vector<std::string> result;
        for (const auto& pair : labels) {
            result.push_back(pair.first);
        }
        return result;
    }
};

#endif // SYMBOL_TABLE_H
