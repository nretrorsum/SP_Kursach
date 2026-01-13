#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.h"

class Lexer {
private:
    std::string source;
    size_t pos;
    int line;
    int column;
    std::vector<std::string> errors;

    char current() const;
    char peek(int offset = 1) const;
    void advance();
    void skipWhitespace();
    void skipComment();
    Token readNumber();
    Token readIdentifierOrKeyword();
    bool isValidIdentifier(const std::string& id) const;

public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();
    const std::vector<std::string>& getErrors() const { return errors; }
};

#endif // LEXER_H
