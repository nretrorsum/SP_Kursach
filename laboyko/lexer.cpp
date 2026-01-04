#include "lexer.h"
#include <cctype>
#include <unordered_map>

static std::unordered_map<std::string, TokenType> keywords = {
    {"#Program", TokenType::HASH_PROGRAM},
    {"Start", TokenType::START},
    {"Stop", TokenType::STOP},
    {"Variable", TokenType::VARIABLE},
    {"Get", TokenType::GET},
    {"Put", TokenType::PUT},
    {"If", TokenType::IF},
    {"Then", TokenType::THEN},
    {"Goto", TokenType::GOTO},
    {"For", TokenType::FOR},
    {"To", TokenType::TO},
    {"Next", TokenType::NEXT},
    {"Integer32_t", TokenType::INTEGER32_T},
    {"Mul", TokenType::MUL},
    {"Div", TokenType::DIV},
    {"Mod", TokenType::MOD},
    {"And", TokenType::AND},
    {"Or", TokenType::OR}
};

Lexer::Lexer(const std::string& source) : source(source), pos(0), line(1), column(1) {}

char Lexer::current() const {
    if (pos >= source.length()) return '\0';
    return source[pos];
}

char Lexer::peek(int offset) const {
    if (pos + offset >= source.length()) return '\0';
    return source[pos + offset];
}

void Lexer::advance() {
    if (current() == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    pos++;
}

void Lexer::skipWhitespace() {
    while (std::isspace(current())) {
        advance();
    }
}

void Lexer::skipComment() {
    // Comment format: @@ ... (single line, no closing)
    if (current() == '@' && peek() == '@') {
        advance(); // @
        advance(); // @
        while (current() != '\0' && current() != '\n') {
            advance();
        }
    }
}

Token Lexer::readNumber() {
    int startCol = column;
    std::string num;

    // Handle negative numbers
    if (current() == '-') {
        num += current();
        advance();
    }

    while (std::isdigit(current())) {
        num += current();
        advance();
    }
    return Token(TokenType::NUMBER, num, line, startCol);
}

Token Lexer::readIdentifierOrKeyword() {
    int startCol = column;
    std::string id;

    // Check for #Program
    if (current() == '#') {
        id += current();
        advance();
    }

    while (std::isalnum(current()) || current() == '_') {
        id += current();
        advance();
    }

    // Check for label (identifier followed by :)
    if (current() == ':' && peek() != ':') {
        // This is a label definition
        advance(); // consume :
        return Token(TokenType::LABEL, id, line, startCol);
    }

    // Check if it's a keyword
    auto it = keywords.find(id);
    if (it != keywords.end()) {
        return Token(it->second, id, line, startCol);
    }

    // Check if valid identifier (Up8 - up to 8 uppercase letters)
    if (isValidIdentifier(id)) {
        return Token(TokenType::IDENTIFIER, id, line, startCol);
    }

    // Check for label reference (all uppercase, used with Goto)
    bool allUpper = true;
    for (char c : id) {
        if (!std::isupper(c) && !std::isdigit(c)) {
            allUpper = false;
            break;
        }
    }
    if (allUpper && id.length() <= 8) {
        return Token(TokenType::IDENTIFIER, id, line, startCol);
    }

    errors.push_back("Line " + std::to_string(line) + ": Invalid identifier '" + id +
                     "' - must be up to 8 uppercase letters");
    return Token(TokenType::UNKNOWN, id, line, startCol);
}

bool Lexer::isValidIdentifier(const std::string& id) const {
    // Up8 - up to 8 uppercase letters
    if (id.empty() || id.length() > 8) return false;
    for (char c : id) {
        if (!std::isupper(c)) return false;
    }
    return true;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (current() != '\0') {
        skipWhitespace();

        if (current() == '\0') break;

        // Skip comments @@ ...
        if (current() == '@' && peek() == '@') {
            skipComment();
            continue;
        }

        int startLine = line;
        int startCol = column;

        // Single character tokens and operators
        if (current() == '+') {
            tokens.push_back(Token(TokenType::PLUS, "+", startLine, startCol));
            advance();
        }
        else if (current() == '-') {
            // Check if it's a negative number
            if (std::isdigit(peek())) {
                tokens.push_back(readNumber());
            } else {
                tokens.push_back(Token(TokenType::MINUS, "-", startLine, startCol));
                advance();
            }
        }
        else if (current() == ';') {
            tokens.push_back(Token(TokenType::SEMICOLON, ";", startLine, startCol));
            advance();
        }
        else if (current() == ',') {
            tokens.push_back(Token(TokenType::COMMA, ",", startLine, startCol));
            advance();
        }
        else if (current() == '(') {
            tokens.push_back(Token(TokenType::LPAREN, "(", startLine, startCol));
            advance();
        }
        else if (current() == ')') {
            tokens.push_back(Token(TokenType::RPAREN, ")", startLine, startCol));
            advance();
        }
        else if (current() == ':') {
            tokens.push_back(Token(TokenType::COLON, ":", startLine, startCol));
            advance();
        }
        // Assignment operator >>
        else if (current() == '>' && peek() == '>') {
            tokens.push_back(Token(TokenType::ASSIGN, ">>", startLine, startCol));
            advance();
            advance();
        }
        // Comparison operators
        else if (current() == '=') {
            tokens.push_back(Token(TokenType::EQ, "=", startLine, startCol));
            advance();
        }
        else if (current() == '<' && peek() == '>') {
            tokens.push_back(Token(TokenType::NE, "<>", startLine, startCol));
            advance();
            advance();
        }
        else if (current() == '!' && peek() == '>') {
            tokens.push_back(Token(TokenType::NOT_GREATER, "!>", startLine, startCol));
            advance();
            advance();
        }
        else if (current() == '!' && peek() == '<') {
            tokens.push_back(Token(TokenType::NOT_LESS, "!<", startLine, startCol));
            advance();
            advance();
        }
        else if (current() == '!') {
            tokens.push_back(Token(TokenType::NOT, "!", startLine, startCol));
            advance();
        }
        // Numbers
        else if (std::isdigit(current())) {
            tokens.push_back(readNumber());
        }
        // Identifiers, keywords, labels
        else if (std::isalpha(current()) || current() == '#' || current() == '_') {
            tokens.push_back(readIdentifierOrKeyword());
        }
        else {
            errors.push_back("Line " + std::to_string(line) + ": Unexpected character '" +
                           std::string(1, current()) + "'");
            advance();
        }
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", line, column));
    return tokens;
}
