/**
 * Course Project on System Programming
 * Topic: Development of a translator for the V07 programming language
 * Variant: Voitovych Oleksandr Viktorovych
 *
 * File: lexer.h
 * Description: Lexical analyzer header file
 */

#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
public:
    Lexer(const std::string& source);

    // Get next token
    Token getNextToken();

    // Get all tokens
    std::vector<Token> tokenize();

    // Check if there are errors
    bool hasErrors() const { return !errors.empty(); }

    // Get error list
    const std::vector<std::string>& getErrors() const { return errors; }

private:
    std::string source;
    size_t pos;
    int line;
    int column;
    std::vector<std::string> errors;

    // Keyword table
    static std::unordered_map<std::string, TokenType> keywords;

    // Helper methods
    char peek() const;
    char peekNext() const;
    char advance();
    bool isAtEnd() const;
    void skipWhitespace();
    void skipComment();

    // Token recognition methods
    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readOperator();

    // Validation
    bool isValidIdentifier(const std::string& id) const;
    bool isValidNumber(const std::string& num) const;

    // Add error
    void addError(const std::string& message);
};

#endif // LEXER_H
