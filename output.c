/* Generated C code from Z07 language */
/* Translator by Zaplyvanyi Daniil */

#include <stdio.h>
#include <stdint.h>

int main() {
    int16_t A, B, SUM, I, MAX, RES;

    A = 10;
    B = 32000;
    MAX = 32767;
    scanf("%hd", &SUM);
    printf("%d\n", MAX);
    SUM = A + B * 2 / 5;
    RES = SUM % 10;
    if (A != 0 && B >= A || (!SUM) == 0) {
        A = A - 1;
    } else {
        A = 0;
    }
    for (I = 1; I <= 10; I++) {
        SUM = SUM + I;
    }

    return 0;
}
