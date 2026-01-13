/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V03 programming language
 * Variant: Voitovych Oleksandr Viktorovych
 *
 * File: lexer.cpp
 * Description: Lexical analyzer implementation
 */

#include "lexer.h"
#include <cctype>
#include <sstream>

// Keyword table initialization (Up-Low case, first character uppercase)
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
    // Comment starts with /* and continues to end of line
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
    ss << "Lexical error [line " << line << ", column " << column << "]: " << message;
    errors.push_back(ss.str());
}

bool Lexer::isValidIdentifier(const std::string& id) const {
    // Identifiers: Up2 - uppercase letters only, max 2 characters
    // First character must be uppercase
    if (id.empty() || id.length() > 2) {
        return false;
    }
    // First character - uppercase letter
    if (!std::isupper(id[0])) {
        return false;
    }
    // Second character (if exists) - uppercase letter
    if (id.length() == 2 && !std::isupper(id[1])) {
        return false;
    }
    return true;
}

bool Lexer::isValidNumber(const std::string& num) const {
    // Check Int16 range: -32768 to 32767
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

    // Read identifier/keyword
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        value += advance();
    }

    // Check if it's a keyword
    auto it = keywords.find(value);
    if (it != keywords.end()) {
        return Token(it->second, value, startLine, startColumn);
    }

    // This is an identifier - check Up2 rules
    if (!isValidIdentifier(value)) {
        addError("Invalid identifier '" + value + "'. Identifier must contain only uppercase letters (max 2 characters)");
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
        addError("Number '" + value + "' is out of Int16 range (-32768..32767)");
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
            // Check for ::=
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
            addError("Unknown operator '>'");
            return Token(TokenType::UNKNOWN, ">", startLine, startColumn);

        case '<':
            if (peek() == '<') {
                advance();
                return Token(TokenType::LT, "<<", startLine, startColumn);
            }
            addError("Unknown operator '<'");
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
            addError(std::string("Unknown character '") + c + "'");
            return Token(TokenType::UNKNOWN, std::string(1, c), startLine, startColumn);
    }
}

Token Lexer::getNextToken() {
    skipWhitespace();

    // Skip comments
    while (peek() == '/' && peekNext() == '*') {
        skipComment();
        skipWhitespace();
    }

    if (isAtEnd()) {
        return Token(TokenType::END_OF_FILE, "", line, column);
    }

    char c = peek();

    // Identifier or keyword
    if (std::isalpha(c)) {
        return readIdentifierOrKeyword();
    }

    // Number
    if (std::isdigit(c)) {
        return readNumber();
    }

    // Operator or delimiter
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
