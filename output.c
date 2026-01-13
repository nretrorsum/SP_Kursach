/* Generated C code from Z07 language */
/* Translator by Zaplyvanyi Daniil */

#include <stdio.h>
#include <stdint.h>

int main() {
    int16_t A, B, I, J, X, SQR;

    scanf("%hd", &A);
    scanf("%hd", &B);
    for (I = A; I <= B; I++) {
        SQR = I * I;
        printf("%d\n", SQR);
    }
    X = 0;
    for (I = 1; I <= A; I++) {
        for (J = 1; J <= B; J++) {
            X = X + 1;
        }
    }
    printf("%d\n", X);

    return 0;
}
