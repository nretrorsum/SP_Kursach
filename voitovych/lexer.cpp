/**
 * Курсовий проект з Системного Програмування
 * Тема: Розробка транслятора з вхідної мови програмування V07
 * Варіант: Войтович Олександр Вікторович
 *
 * Файл: lexer.cpp
 * Опис: Реалізація лексичного аналізатора
 */

#include "lexer.h"
#include <cctype>
#include <sstream>

// Ініціалізація таблиці ключових слів (Up-Low case, перший символ Up)
std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"Program", TokenType::PROGRAM},
    {"Var", TokenType::VAR},
    {"Begin", TokenType::BEGIN},
    {"End", TokenType::END},
    {"If", TokenType::IF},
    {"Goto", TokenType::GOTO},
    {"For", TokenType::FOR},
    {"To", TokenType::TO},
    {"Next", TokenType::NEXT},
    {"Get", TokenType::GET},
    {"Put", TokenType::PUT},
    {"Int16", TokenType::INT16},
    {"And", TokenType::AND},
    {"Or", TokenType::OR},
    {"Mul", TokenType::MUL},
    {"Div", TokenType::DIV},
    {"Mod", TokenType::MOD},
    {"Eg", TokenType::EG},
    {"Ne", TokenType::NE}
};

Lexer::Lexer(const std::string& source)
    : source(source), pos(0), line(1), column(1) {}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[pos];
}

char Lexer::peekNext() const {
    if (pos + 1 >= source.length()) return '\0';
    return source[pos + 1];
}

char Lexer::advance() {
    char c = source[pos++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

bool Lexer::isAtEnd() const {
    return pos >= source.length();
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else {
            break;
        }
    }
}

void Lexer::skipComment() {
    // Коментар починається з /* і продовжується до кінця рядка
    if (peek() == '/' && peekNext() == '*') {
        advance(); // /
        advance(); // *
        while (!isAtEnd() && peek() != '\n') {
            advance();
        }
    }
}

void Lexer::addError(const std::string& message) {
    std::stringstream ss;
    ss << "Лексична помилка [рядок " << line << ", стовпець " << column << "]: " << message;
    errors.push_back(ss.str());
}

bool Lexer::isValidIdentifier(const std::string& id) const {
    // Ідентифікатори: Up2 - тільки великі букви, максимум 2 символи
    // Перший символ має бути великою буквою
    if (id.empty() || id.length() > 2) {
        return false;
    }
    // Перший символ - велика буква
    if (!std::isupper(id[0])) {
        return false;
    }
    // Другий символ (якщо є) - велика буква
    if (id.length() == 2 && !std::isupper(id[1])) {
        return false;
    }
    return true;
}

bool Lexer::isValidNumber(const std::string& num) const {
    // Перевірка діапазону Int16: -32768 до 32767
    try {
        long value = std::stol(num);
        return value >= -32768 && value <= 32767;
    } catch (...) {
        return false;
    }
}

Token Lexer::readIdentifierOrKeyword() {
    int startLine = line;
    int startColumn = column;
    std::string value;

    // Читаємо ідентифікатор/ключове слово
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        value += advance();
    }

    // Перевіряємо чи це ключове слово
    auto it = keywords.find(value);
    if (it != keywords.end()) {
        return Token(it->second, value, startLine, startColumn);
    }

    // Це ідентифікатор - перевіряємо правила Up2
    if (!isValidIdentifier(value)) {
        addError("Невірний ідентифікатор '" + value + "'. Ідентифікатор повинен містити тільки великі букви (макс. 2 символи)");
        return Token(TokenType::UNKNOWN, value, startLine, startColumn);
    }

    return Token(TokenType::IDENTIFIER, value, startLine, startColumn);
}

Token Lexer::readNumber() {
    int startLine = line;
    int startColumn = column;
    std::string value;

    while (!isAtEnd() && std::isdigit(peek())) {
        value += advance();
    }

    if (!isValidNumber(value)) {
        addError("Число '" + value + "' виходить за межі діапазону Int16 (-32768..32767)");
        return Token(TokenType::UNKNOWN, value, startLine, startColumn);
    }

    return Token(TokenType::NUMBER, value, startLine, startColumn);
}

Token Lexer::readOperator() {
    int startLine = line;
    int startColumn = column;

    char c = advance();

    switch (c) {
        case ':':
            // Перевіряємо на ::=
            if (peek() == ':' && peekNext() == '=') {
                advance(); // :
                advance(); // =
                return Token(TokenType::ASSIGN, "::=", startLine, startColumn);
            }
            return Token(TokenType::COLON, ":", startLine, startColumn);

        case '>':
            if (peek() == '>') {
                advance();
                return Token(TokenType::GT, ">>", startLine, startColumn);
            }
            addError("Невідомий оператор '>'");
            return Token(TokenType::UNKNOWN, ">", startLine, startColumn);

        case '<':
            if (peek() == '<') {
                advance();
                return Token(TokenType::LT, "<<", startLine, startColumn);
            }
            addError("Невідомий оператор '<'");
            return Token(TokenType::UNKNOWN, "<", startLine, startColumn);

        case '+':
            return Token(TokenType::PLUS, "+", startLine, startColumn);

        case '-':
            return Token(TokenType::MINUS, "-", startLine, startColumn);

        case '!':
            return Token(TokenType::NOT, "!", startLine, startColumn);

        case ';':
            return Token(TokenType::SEMICOLON, ";", startLine, startColumn);

        case ',':
            return Token(TokenType::COMMA, ",", startLine, startColumn);

        case '(':
            return Token(TokenType::LPAREN, "(", startLine, startColumn);

        case ')':
            return Token(TokenType::RPAREN, ")", startLine, startColumn);

        default:
            addError(std::string("Невідомий символ '") + c + "'");
            return Token(TokenType::UNKNOWN, std::string(1, c), startLine, startColumn);
    }
}

Token Lexer::getNextToken() {
    skipWhitespace();

    // Пропускаємо коментарі
    while (peek() == '/' && peekNext() == '*') {
        skipComment();
        skipWhitespace();
    }

    if (isAtEnd()) {
        return Token(TokenType::END_OF_FILE, "", line, column);
    }

    char c = peek();

    // Ідентифікатор або ключове слово
    if (std::isalpha(c)) {
        return readIdentifierOrKeyword();
    }

    // Число
    if (std::isdigit(c)) {
        return readNumber();
    }

    // Оператор або розділювач
    return readOperator();
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        Token token = getNextToken();
        tokens.push_back(token);

        if (token.type == TokenType::END_OF_FILE) {
            break;
        }
    }

    return tokens;
}
