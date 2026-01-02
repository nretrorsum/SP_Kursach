#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <vector>

class Lexer {
private:
    string src;
    int pos;
    int line;
    int len;

    char peek(int offset);
    void skipComment();
    Token readNumber();
    Token readWord();

public:
    Lexer(string source);
    vector<Token> tokenize();
};

#endif