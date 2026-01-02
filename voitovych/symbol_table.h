/**
 * Курсовий проект з Системного Програмування
 * Тема: Розробка транслятора з вхідної мови програмування V07
 * Варіант: Войтович Олександр Вікторович
 *
 * Файл: symbol_table.h
 * Опис: Таблиця символів для зберігання інформації про змінні та мітки
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>

// Тип символу
enum class SymbolType {
    VARIABLE,
    LABEL
};

// Інформація про символ
struct SymbolInfo {
    std::string name;
    SymbolType type;
    std::string dataType;  // Int16
    int declarationLine;
    bool isInitialized;
    bool isDefined;        // Для міток - чи визначена мітка

    SymbolInfo(const std::string& n = "", SymbolType t = SymbolType::VARIABLE,
               const std::string& dt = "Int16", int line = 0)
        : name(n), type(t), dataType(dt), declarationLine(line),
          isInitialized(false), isDefined(false) {}
};

// Таблиця символів
class SymbolTable {
public:
    // Додати змінну
    bool addVariable(const std::string& name, int line) {
        if (variables.find(name) != variables.end()) {
            return false; // Змінна вже існує
        }
        variables[name] = SymbolInfo(name, SymbolType::VARIABLE, "Int16", line);
        variableOrder.push_back(name);
        return true;
    }

    // Додати мітку
    bool addLabel(const std::string& name, int line) {
        if (labels.find(name) != labels.end()) {
            // Мітка вже використовується, позначаємо як визначену
            labels[name].isDefined = true;
            return true;
        }
        SymbolInfo info(name, SymbolType::LABEL, "", line);
        info.isDefined = true;
        labels[name] = info;
        labelOrder.push_back(name);
        return true;
    }

    // Зареєструвати використання мітки (для Goto)
    void useLabel(const std::string& name, int line) {
        if (labels.find(name) == labels.end()) {
            SymbolInfo info(name, SymbolType::LABEL, "", line);
            info.isDefined = false;
            labels[name] = info;
            labelOrder.push_back(name);
        }
    }

    // Перевірити чи існує змінна
    bool hasVariable(const std::string& name) const {
        return variables.find(name) != variables.end();
    }

    // Перевірити чи існує мітка
    bool hasLabel(const std::string& name) const {
        return labels.find(name) != labels.end();
    }

    // Перевірити чи мітка визначена
    bool isLabelDefined(const std::string& name) const {
        auto it = labels.find(name);
        if (it != labels.end()) {
            return it->second.isDefined;
        }
        return false;
    }

    // Отримати інформацію про змінну
    SymbolInfo* getVariable(const std::string& name) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    // Отримати інформацію про мітку
    SymbolInfo* getLabel(const std::string& name) {
        auto it = labels.find(name);
        if (it != labels.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    // Позначити змінну як ініціалізовану
    void setInitialized(const std::string& name) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            it->second.isInitialized = true;
        }
    }

    // Перевірити чи змінна ініціалізована
    bool isInitialized(const std::string& name) const {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return it->second.isInitialized;
        }
        return false;
    }

    // Отримати всі змінні (у порядку оголошення)
    const std::vector<std::string>& getVariableOrder() const {
        return variableOrder;
    }

    // Отримати всі мітки
    const std::vector<std::string>& getLabelOrder() const {
        return labelOrder;
    }

    // Отримати всі невизначені мітки
    std::vector<std::string> getUndefinedLabels() const {
        std::vector<std::string> undefined;
        for (const auto& pair : labels) {
            if (!pair.second.isDefined) {
                undefined.push_back(pair.first);
            }
        }
        return undefined;
    }

    // Отримати map змінних
    const std::unordered_map<std::string, SymbolInfo>& getVariables() const {
        return variables;
    }

    // Отримати map міток
    const std::unordered_map<std::string, SymbolInfo>& getLabels() const {
        return labels;
    }

    // Очистити таблицю
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
