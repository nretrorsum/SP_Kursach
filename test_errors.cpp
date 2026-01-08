#include <iostream>
#include "lexer.h"
using namespace std;

void test(const string& desc, const string& code) {
    cout << "\n=== " << desc << " ===" << endl;
    cout << "Code: " << code << endl;
    Lexer lexer(code);
    vector<Token> tokens = lexer.tokenize();
    cout << "Result: " << tokens.size() << " tokens" << endl;
}

int main() {
    // Тест 1: ідентифікатор не починається з великої
    test("lowercase start", "var int16_t sum");
    
    // Тест 2: ідентифікатор > 4 символів
    test("too long", "var int16_t TOTAL");
    
    // Тест 3: змішаний регістр (має бути помилка!)
    test("mixed case", "var int16_t Sum");
    
    // Тест 4: число > 32767
    test("number overflow", "A <- 50000");
    
    // Тест 5: правильний ідентифікатор
    test("valid ID", "var int16_t SUM");
    
    return 0;
}
