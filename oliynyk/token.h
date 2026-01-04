/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
 *
 * File: token.h
 * Description: Token type definitions for lexical analyzer
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// Token types for O16 language
enum class TokenType {
    // Keywords (lowercase)
    STARTPROGRAM,   // startprogram
    VARIABLE,       // variable
    STARTBLOK,      // startblok
    ENDBLOK,        // endblok
    IF,             // if
    ELSE,           // else
    WHILE,          // while
    GET,            // get
    PUT,            // put
    INT_2,          // int_2

    // Comparison operators (keywords)
    EQ,             // eq (equal)
    NOTEQ,          // noteq (not equal)
    LESS,           // less (less than)
    GR,             // gr (greater than)

    // Logical operators (keywords)
    AND,            // and
    OR,             // or

    // Operators
    ASSIGN,         // ::
    PLUS,           // +
    MINUS,          // -
    MULTIPLY,       // *
    DIVIDE,         // /
    MODULO,         // %
    NOT,            // !

    // Delimiters
    SEMICOLON,      // ;
    COMMA,          // ,
    LPAREN,         // (
    RPAREN,         // )

    // Identifier and number
    IDENTIFIER,     // Up4 with _ prefix - uppercase letters, max 4 characters, starts with _
    NUMBER,         // Integer number

    // Service tokens
    END_OF_FILE,    // End of file
    UNKNOWN         // Unknown token
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType t = TokenType::UNKNOWN, const std::string& v = "", int l = 0, int c = 0)
        : type(t), value(v), line(l), column(c) {}
};

// Function to get token name
inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::STARTPROGRAM: return "STARTPROGRAM";
        case TokenType::VARIABLE: return "VARIABLE";
        case TokenType::STARTBLOK: return "STARTBLOK";
        case TokenType::ENDBLOK: return "ENDBLOK";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::GET: return "GET";
        case TokenType::PUT: return "PUT";
        case TokenType::INT_2: return "INT_2";
        case TokenType::EQ: return "EQ";
        case TokenType::NOTEQ: return "NOTEQ";
        case TokenType::LESS: return "LESS";
        case TokenType::GR: return "GR";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::MODULO: return "MODULO";
        case TokenType::NOT: return "NOT";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNDEFINED";
    }
}

#endif // TOKEN_H
