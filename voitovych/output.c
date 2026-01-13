/**
 * Generated code from V03 language
 * Program: LP
 * Variant: Voitovych Oleksandr Viktorovych
 */

#include <stdio.h>
#include <stdint.h>

int main() {
    int16_t A, B, I, J, X;
    
    scanf("%hd", &A);
    scanf("%hd", &B);
    for (I = A; I <= B; I++) {
        printf("%d\n", (I * I));
    }
    X = 0;
    for (I = 1; I <= A; I++) {
        for (J = 1; J <= B; J++) {
            X = (X + 1);
        }
    }
    printf("%d\n", X);
    
    return 0;
}
