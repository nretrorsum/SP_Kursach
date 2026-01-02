/**
 * Курсовий проект з Системного Програмування
 * Тема: Розробка транслятора з вхідної мови програмування V07
 * Варіант: Войтович Олександр Вікторович
 *
 * Файл: token.h
 * Опис: Визначення типів токенів для лексичного аналізатора
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// Типи токенів для мови V07
enum class TokenType {
    // Ключові слова (Up-Low case)
    PROGRAM,        // Program
    VAR,            // Var
    BEGIN,          // Begin
    END,            // End
    IF,             // If
    GOTO,           // Goto
    FOR,            // For
    TO,             // To
    NEXT,           // Next
    GET,            // Get
    PUT,            // Put
    INT16,          // Int16

    // Логічні оператори (ключові слова)
    AND,            // And
    OR,             // Or

    // Арифметичні оператори (ключові слова)
    MUL,            // Mul
    DIV,            // Div
    MOD,            // Mod

    // Оператори порівняння (ключові слова)
    EG,             // Eg (equal)
    NE,             // Ne (not equal)

    // Оператори
    ASSIGN,         // ::=
    PLUS,           // +
    MINUS,          // -
    NOT,            // !
    GT,             // >> (greater than)
    LT,             // << (less than)

    // Розділювачі
    SEMICOLON,      // ;
    COMMA,          // ,
    COLON,          // :
    LPAREN,         // (
    RPAREN,         // )

    // Ідентифікатор та число
    IDENTIFIER,     // Up2 - великі букви, макс 2 символи
    NUMBER,         // Ціле число

    // Мітка для Goto
    LABEL,          // Мітка (ідентифікатор з двокрапкою)

    // Службові
    END_OF_FILE,    // Кінець файлу
    UNKNOWN         // Невідомий токен
};

// Структура токена
struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType t = TokenType::UNKNOWN, const std::string& v = "", int l = 0, int c = 0)
        : type(t), value(v), line(l), column(c) {}
};

// Функція для отримання назви токена
inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::PROGRAM: return "PROGRAM";
        case TokenType::VAR: return "VAR";
        case TokenType::BEGIN: return "BEGIN";
        case TokenType::END: return "END";
        case TokenType::IF: return "IF";
        case TokenType::GOTO: return "GOTO";
        case TokenType::FOR: return "FOR";
        case TokenType::TO: return "TO";
        case TokenType::NEXT: return "NEXT";
        case TokenType::GET: return "GET";
        case TokenType::PUT: return "PUT";
        case TokenType::INT16: return "INT16";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::MOD: return "MOD";
        case TokenType::EG: return "EG";
        case TokenType::NE: return "NE";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::NOT: return "NOT";
        case TokenType::GT: return "GT";
        case TokenType::LT: return "LT";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::COLON: return "COLON";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::LABEL: return "LABEL";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNDEFINED";
    }
}

#endif // TOKEN_H
