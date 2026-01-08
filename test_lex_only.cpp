#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"

using namespace std;

int main() {
    string code = "program Test;\nvar int16_t Sum, Max\n\nstart\n    Sum <- 10\nfinish";
    
    Lexer lexer(code);
    vector<Token> tokens = lexer.tokenize();
    
    for (const auto& token : tokens) {
        if (token.type != END_OF_FILE) {
            cout << "Token: " << tokenTypeToString(token.type) << " = '" << token.value << "'" << endl;
        }
    }
    
    return 0;
}
