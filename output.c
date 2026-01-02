/* Generated C code from Z07 language */
/* Translator by Zaplyvanyi Daniil */

#include <stdio.h>
#include <stdint.h>

int main() {
    int16_t A, B, Sum, I, Max, Res;

    A = 10;
    B = 32000;
    Max = 32767;
    scanf("%hd", &Sum);
    printf("%d\n", Max);
    Sum = A + B * 2 / 5;
    Res = Sum % 10;
    if (A != 0 && B >= A || (!Sum) == 0) {
        A = A - 1;
    } else {
        A = 0;
    }
    for (I = 1; I <= 10; I++) {
        Sum = Sum + I;
    }

    return 0;
}
