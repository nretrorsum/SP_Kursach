#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

using namespace std;

enum TokenType {
    K_PROGRAM, K_VAR, K_START, K_FINISH,
    K_GET, K_PUT, K_IF, K_ELSE, K_FOR, K_IN,
    K_INT16_T,
    OP_ASSIGN, OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_MOD,
    OP_EG, OP_NE, OP_GE, OP_LE, OP_NOT, OP_AND, OP_OR,
    SEMICOLON, COMMA, RANGE, LPAREN, RPAREN,
    IDENTIFIER, NUMBER, END_OF_FILE, UNKNOWN
};

struct Token {
    TokenType type;
    string value;
    int line;
};

inline string tokenTypeToString(TokenType type) {
    switch(type) {
        case K_PROGRAM: return "PROGRAM";
        case K_VAR: return "VAR";
        case K_START: return "START";
        case K_FINISH: return "FINISH";
        case K_GET: return "GET";
        case K_PUT: return "PUT";
        case K_IF: return "IF";
        case K_ELSE: return "ELSE";
        case K_FOR: return "FOR";
        case K_IN: return "IN";
        case K_INT16_T: return "INT16_T";
        case OP_ASSIGN: return "ASSIGN(<-)";
        case OP_PLUS: return "PLUS";
        case OP_MINUS: return "MINUS";
        case OP_MUL: return "MUL";
        case OP_DIV: return "DIV";
        case OP_MOD: return "MOD";
        case OP_EG: return "EG";
        case OP_NE: return "NE";
        case OP_GE: return "GE(>=)";
        case OP_LE: return "LE(<=)";
        case OP_NOT: return "NOT";
        case OP_AND: return "AND";
        case OP_OR: return "OR";
        case SEMICOLON: return "SEMICOLON";
        case COMMA: return "COMMA";
        case RANGE: return "RANGE(..)";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case IDENTIFIER: return "ID";
        case NUMBER: return "NUMBER";
        case END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}

#endif