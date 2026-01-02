/**
 * Курсовий проект з Системного Програмування
 * Тема: Розробка транслятора з вхідної мови програмування V07
 * Варіант: Войтович Олександр Вікторович
 *
 * Файл: lexer.h
 * Опис: Заголовний файл лексичного аналізатора
 */

#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
public:
    Lexer(const std::string& source);

    // Отримати наступний токен
    Token getNextToken();

    // Отримати всі токени
    std::vector<Token> tokenize();

    // Перевірити чи є помилки
    bool hasErrors() const { return !errors.empty(); }

    // Отримати список помилок
    const std::vector<std::string>& getErrors() const { return errors; }

private:
    std::string source;
    size_t pos;
    int line;
    int column;
    std::vector<std::string> errors;

    // Таблиця ключових слів
    static std::unordered_map<std::string, TokenType> keywords;

    // Допоміжні методи
    char peek() const;
    char peekNext() const;
    char advance();
    bool isAtEnd() const;
    void skipWhitespace();
    void skipComment();

    // Методи розпізнавання токенів
    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readOperator();

    // Валідація
    bool isValidIdentifier(const std::string& id) const;
    bool isValidNumber(const std::string& num) const;

    // Додавання помилки
    void addError(const std::string& message);
};

#endif // LEXER_H
