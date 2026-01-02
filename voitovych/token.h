/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V07 programming language
 * Variant: Voitovych Oleksandr Viktorovych
 *
 * File: token.h
 * Description: Token type definitions for lexical analyzer
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// Token types for V07 language
enum class TokenType {
    // Keywords (Up-Low case)
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

    // Logical operators (keywords)
    AND,            // And
    OR,             // Or

    // Arithmetic operators (keywords)
    MUL,            // Mul
    DIV,            // Div
    MOD,            // Mod

    // Comparison operators (keywords)
    EG,             // Eg (equal)
    NE,             // Ne (not equal)

    // Operators
    ASSIGN,         // ::=
    PLUS,           // +
    MINUS,          // -
    NOT,            // !
    GT,             // >> (greater than)
    LT,             // << (less than)

    // Delimiters
    SEMICOLON,      // ;
    COMMA,          // ,
    COLON,          // :
    LPAREN,         // (
    RPAREN,         // )

    // Identifier and number
    IDENTIFIER,     // Up2 - uppercase letters, max 2 characters
    NUMBER,         // Integer number

    // Label for Goto
    LABEL,          // Label (identifier with colon)

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
