/**
 * Course Project on System Programming
 * Topic: Development of a translator for the O16 programming language
 * Variant: Oliynyk Bohdan Romanovych
 *
 * File: lexer.h
 * Description: Lexical analyzer header file
 */

#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>

class Lexer {
public:
    explicit Lexer(const std::string& source);

    // Main tokenization method
    std::vector<Token> tokenize();

    // Error handling
    bool hasErrors() const;
    const std::vector<std::string>& getErrors() const;

private:
    std::string source;
    size_t pos;
    int line;
    int column;
    std::vector<std::string> errors;

    // Helper methods
    char currentChar() const;
    char peekChar() const;
    void advance();
    void skipWhitespace();
    void skipComment();

    // Token recognition
    Token scanToken();
    Token scanIdentifierOrKeyword();
    Token scanNumber();
    Token scanOperator();

    // Validation
    bool isValidIdentifier(const std::string& id) const;

    // Error reporting
    void addError(const std::string& message);
};

#endif // LEXER_H
