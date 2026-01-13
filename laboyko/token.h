#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    // Program structure
    HASH_PROGRAM,   // #Program
    START,          // Start
    STOP,           // Stop
    VARIABLE,       // Variable

    // I/O
    GET,            // Get
    PUT,            // Put

    // Control flow - Basic style
    IF,             // If
    THEN,           // Then
    GOTO,           // Goto
    FOR,            // For
    TO,             // To
    NEXT,           // Next

    // Data type
    INTEGER32_T,    // Integer32_t

    // Arithmetic operators
    PLUS,           // +
    MINUS,          // -
    MUL,            // Mul
    DIV,            // Div
    MOD,            // Mod

    // Comparison operators
    EQ,             // =
    NE,             // <>
    NOT_GREATER,    // !>  (not greater, means <=)
    NOT_LESS,       // !<  (not less, means >=)

    // Logical operators
    NOT,            // !
    AND,            // And
    OR,             // Or

    // Assignment
    ASSIGN,         // >>

    // Delimiters
    SEMICOLON,      // ;
    COMMA,          // ,
    LPAREN,         // (
    RPAREN,         // )
    COLON,          // :

    // Literals and identifiers
    IDENTIFIER,     // Up to 8 uppercase letters
    NUMBER,         // Integer literal
    LABEL,          // Label for Goto

    // Special
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType t = TokenType::UNKNOWN, const std::string& v = "", int l = 1, int c = 1)
        : type(t), value(v), line(l), column(c) {}
};

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::HASH_PROGRAM: return "HASH_PROGRAM";
        case TokenType::START: return "START";
        case TokenType::STOP: return "STOP";
        case TokenType::VARIABLE: return "VARIABLE";
        case TokenType::GET: return "GET";
        case TokenType::PUT: return "PUT";
        case TokenType::IF: return "IF";
        case TokenType::THEN: return "THEN";
        case TokenType::GOTO: return "GOTO";
        case TokenType::FOR: return "FOR";
        case TokenType::TO: return "TO";
        case TokenType::NEXT: return "NEXT";
        case TokenType::INTEGER32_T: return "INTEGER32_T";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::MOD: return "MOD";
        case TokenType::EQ: return "EQ";
        case TokenType::NE: return "NE";
        case TokenType::NOT_GREATER: return "NOT_GREATER";
        case TokenType::NOT_LESS: return "NOT_LESS";
        case TokenType::NOT: return "NOT";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::COLON: return "COLON";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::LABEL: return "LABEL";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

#endif // TOKEN_H
