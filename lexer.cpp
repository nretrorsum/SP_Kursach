#include "lexer.h"
#include <cctype>

Lexer::Lexer(string source) : src(source), pos(0), line(1) {
    len = source.length();
}

char Lexer::peek(int offset) {
    if (pos + offset >= len) return '\0';
    return src[pos + offset];
}

void Lexer::skipComment() {
    pos += 2;
    while (pos < len) {
        if (src[pos] == '%' && peek(1) == '%') {
            pos += 2; return;
        }
        if (src[pos] == '\n') line++;
        pos++;
    }
}

Token Lexer::readNumber() {
    int start = pos;
    while (pos < len && isdigit(src[pos])) pos++;
    
    string numStr = src.substr(start, pos - start);
    long value = stol(numStr);
    if (value > 32767) {
        cout << "Lexical Error (Line " << line << "): Number '" << numStr 
             << "' exceeds int16_t range (max 32767)." << endl;
        return {UNKNOWN, numStr, line}; 
    }

    return {NUMBER, numStr, line};
}

Token Lexer::readWord() {
    int start = pos;
    
    while (pos < len && (isalnum(src[pos]) || src[pos] == '_')) {
        pos++;
    }
    
    string word = src.substr(start, pos - start);
    
    if (word == "program") return {K_PROGRAM, word, line};
    if (word == "var")     return {K_VAR, word, line};
    if (word == "start")   return {K_START, word, line};
    if (word == "finish")  return {K_FINISH, word, line};
    if (word == "get")     return {K_GET, word, line};
    if (word == "put")     return {K_PUT, word, line};
    if (word == "if")      return {K_IF, word, line};
    if (word == "else")    return {K_ELSE, word, line};
    if (word == "for")     return {K_FOR, word, line};
    if (word == "in")      return {K_IN, word, line};
    
    if (word == "int16_t") return {K_INT16_T, word, line};
    
    if (word == "eg")  return {OP_EG, word, line};
    if (word == "ne")  return {OP_NE, word, line};
    if (word == "and") return {OP_AND, word, line};
    if (word == "or")  return {OP_OR, word, line};
        
    // Up4: тільки великі букви, максимум 4 символи
    if (word.length() > 4) {
        cout << "Lexical Error (Line " << line << "): Identifier '" << word
             << "' is too long. Max length is 4 characters." << endl;
        return {UNKNOWN, word, line};
    }

    // Перевірка що ВСІ символи - великі букви
    for (char c : word) {
        if (!isupper(c)) {
            cout << "Lexical Error (Line " << line << "): Identifier '" << word
                 << "' is invalid. Must contain only UPPERCASE letters (A-Z)." << endl;
            return {UNKNOWN, word, line};
        }
    }

    return {IDENTIFIER, word, line};
}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    while (pos < len) {
        char current = src[pos];
        if (isspace(current)) {
            if (current == '\n') line++;
            pos++;
            continue;
        }
        if (current == '%' && peek(1) == '%') {
            skipComment();
            continue;
        }
        if (isdigit(current)) {
            tokens.push_back(readNumber());
            continue;
        }
        if (isalpha(current)) {
            tokens.push_back(readWord());
            continue;
        }
        switch (current) {
            case ';': tokens.push_back({SEMICOLON, ";", line}); pos++; break;
            case ',': tokens.push_back({COMMA, ",", line}); pos++; break;
            case '(': tokens.push_back({LPAREN, "(", line}); pos++; break;
            case ')': tokens.push_back({RPAREN, ")", line}); pos++; break;
            case '+': tokens.push_back({OP_PLUS, "+", line}); pos++; break;
            case '-': tokens.push_back({OP_MINUS, "-", line}); pos++; break;
            case '*': tokens.push_back({OP_MUL, "*", line}); pos++; break;
            case '/': tokens.push_back({OP_DIV, "/", line}); pos++; break;
            case '%': tokens.push_back({OP_MOD, "%", line}); pos++; break;
            case '!': tokens.push_back({OP_NOT, "!", line}); pos++; break;
            case '<':
                if (peek(1) == '-') {
                    tokens.push_back({OP_ASSIGN, "<-", line});
                    pos += 2;
                } else if (peek(1) == '=') {
                    tokens.push_back({OP_LE, "<=", line});
                    pos += 2;
                } else {
                    cout << "Lexer Error: Unknown operator '<' at line " << line << ". Use '<=' or '<-'" << endl;
                    pos++;
                }
                break;
            case '>':
                if (peek(1) == '=') {
                    tokens.push_back({OP_GE, ">=", line});
                    pos += 2;
                } else {
                    cout << "Lexer Error: Unknown operator '>' at line " << line << ". Use '>='" << endl;
                    pos++;
                }
                break;
            case '.':
                if (peek(1) == '.') {
                    tokens.push_back({RANGE, "..", line});
                    pos += 2;
                } else {
                    tokens.push_back({UNKNOWN, string(1, current), line});
                    pos++;
                }
                break;
            default:
                cout << "Lexer Error: Unknown char '" << current << "' at line " << line << endl;
                pos++;
                break;
        }
    }
    tokens.push_back({END_OF_FILE, "", line});
    return tokens;
}