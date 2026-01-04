/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
 *
 * File: lexer.cpp
 * Description: Lexical analyzer implementation
 */

#include "lexer.h"
#include <cctype>
#include <unordered_map>

// Keywords map (all lowercase)
static const std::unordered_map<std::string, TokenType> keywords = {
    {"startprogram", TokenType::STARTPROGRAM},
    {"variable", TokenType::VARIABLE},
    {"startblok", TokenType::STARTBLOK},
    {"endblok", TokenType::ENDBLOK},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"get", TokenType::GET},
    {"put", TokenType::PUT},
    {"int_2", TokenType::INT_2},
    {"eq", TokenType::EQ},
    {"noteq", TokenType::NOTEQ},
    {"less", TokenType::LESS},
    {"gr", TokenType::GR},
    {"and", TokenType::AND},
    {"or", TokenType::OR}
};

Lexer::Lexer(const std::string& source)
    : source(source), pos(0), line(1), column(1) {}

char Lexer::currentChar() const {
    if (pos >= source.length()) return '\0';
    return source[pos];
}

char Lexer::peekChar() const {
    if (pos + 1 >= source.length()) return '\0';
    return source[pos + 1];
}

void Lexer::advance() {
    if (pos < source.length()) {
        if (source[pos] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        pos++;
    }
}

void Lexer::skipWhitespace() {
    while (pos < source.length() && std::isspace(currentChar())) {
        advance();
    }
}

void Lexer::skipComment() {
    // Comment format: /* ... (single line, no closing)
    if (currentChar() == '/' && peekChar() == '*') {
        advance(); // skip /
        advance(); // skip *
        // Skip until end of line
        while (currentChar() != '\0' && currentChar() != '\n') {
            advance();
        }
    }
}

bool Lexer::isValidIdentifier(const std::string& id) const {
    // Identifier format: Up4 with _ prefix
    // Must start with _, followed by up to 4 uppercase letters
    if (id.empty() || id[0] != '_') {
        return false;
    }

    if (id.length() < 2 || id.length() > 5) { // _ + 1-4 chars
        return false;
    }

    for (size_t i = 1; i < id.length(); i++) {
        if (!std::isupper(id[i])) {
            return false;
        }
    }

    return true;
}

Token Lexer::scanIdentifierOrKeyword() {
    int startColumn = column;
    std::string value;

    // Check if it starts with underscore (identifier)
    if (currentChar() == '_') {
        value += currentChar();
        advance();

        // Read uppercase letters (max 4)
        while (std::isupper(currentChar())) {
            value += currentChar();
            advance();
        }

        // Validate identifier
        if (!isValidIdentifier(value)) {
            addError("Line " + std::to_string(line) + ", Column " + std::to_string(startColumn) +
                    ": Invalid identifier '" + value + "'. Must be _ followed by 1-4 uppercase letters.");
            return Token(TokenType::UNKNOWN, value, line, startColumn);
        }

        return Token(TokenType::IDENTIFIER, value, line, startColumn);
    }

    // Read word (letters, digits, underscore for keywords like int_2)
    while (std::isalnum(currentChar()) || currentChar() == '_') {
        value += currentChar();
        advance();
    }

    // Check if it's a keyword
    auto it = keywords.find(value);
    if (it != keywords.end()) {
        return Token(it->second, value, line, startColumn);
    }

    // Not a keyword and not a valid identifier starting with _
    addError("Line " + std::to_string(line) + ", Column " + std::to_string(startColumn) +
            ": Invalid identifier '" + value + "'. Identifiers must start with _ followed by 1-4 uppercase letters.");
    return Token(TokenType::UNKNOWN, value, line, startColumn);
}

Token Lexer::scanNumber() {
    int startColumn = column;
    std::string value;

    // Handle negative numbers
    if (currentChar() == '-') {
        value += currentChar();
        advance();
    }

    while (std::isdigit(currentChar())) {
        value += currentChar();
        advance();
    }

    // Check for int_2 range (-32768 to 32767)
    try {
        long num = std::stol(value);
        if (num < -32768 || num > 32767) {
            addError("Line " + std::to_string(line) + ", Column " + std::to_string(startColumn) +
                    ": Number " + value + " is out of int_2 range (-32768 to 32767).");
        }
    } catch (...) {
        addError("Line " + std::to_string(line) + ", Column " + std::to_string(startColumn) +
                ": Invalid number format '" + value + "'.");
    }

    return Token(TokenType::NUMBER, value, line, startColumn);
}

Token Lexer::scanOperator() {
    int startColumn = column;
    char c = currentChar();

    // Two-character operators
    if (c == ':' && peekChar() == ':') {
        advance();
        advance();
        return Token(TokenType::ASSIGN, "::", line, startColumn);
    }

    // Single-character operators
    advance();
    switch (c) {
        case '+': return Token(TokenType::PLUS, "+", line, startColumn);
        case '-': return Token(TokenType::MINUS, "-", line, startColumn);
        case '*': return Token(TokenType::MULTIPLY, "*", line, startColumn);
        case '/': return Token(TokenType::DIVIDE, "/", line, startColumn);
        case '%': return Token(TokenType::MODULO, "%", line, startColumn);
        case '!': return Token(TokenType::NOT, "!", line, startColumn);
        case ';': return Token(TokenType::SEMICOLON, ";", line, startColumn);
        case ',': return Token(TokenType::COMMA, ",", line, startColumn);
        case '(': return Token(TokenType::LPAREN, "(", line, startColumn);
        case ')': return Token(TokenType::RPAREN, ")", line, startColumn);
        default:
            addError("Line " + std::to_string(line) + ", Column " + std::to_string(startColumn) +
                    ": Unknown character '" + std::string(1, c) + "'.");
            return Token(TokenType::UNKNOWN, std::string(1, c), line, startColumn);
    }
}

Token Lexer::scanToken() {
    skipWhitespace();

    // Check for comment
    if (currentChar() == '/' && peekChar() == '*') {
        skipComment();
        skipWhitespace();
    }

    if (currentChar() == '\0') {
        return Token(TokenType::END_OF_FILE, "", line, column);
    }

    // Identifier starting with _
    if (currentChar() == '_') {
        return scanIdentifierOrKeyword();
    }

    // Keyword or invalid identifier (starts with letter)
    if (std::isalpha(currentChar())) {
        return scanIdentifierOrKeyword();
    }

    // Number
    if (std::isdigit(currentChar())) {
        return scanNumber();
    }

    // Operators and delimiters
    return scanOperator();
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        Token token = scanToken();
        tokens.push_back(token);

        if (token.type == TokenType::END_OF_FILE) {
            break;
        }
    }

    return tokens;
}

bool Lexer::hasErrors() const {
    return !errors.empty();
}

const std::vector<std::string>& Lexer::getErrors() const {
    return errors;
}

void Lexer::addError(const std::string& message) {
    errors.push_back(message);
}
