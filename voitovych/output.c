/**
 * Generated code from V03 language
 * Program: LN
 * Variant: Voitovych Oleksandr Viktorovych
 */

#include <stdio.h>
#include <stdint.h>

int main() {
    int16_t A, B, X, Y;
    
    scanf("%hd", &A);
    scanf("%hd", &B);
    printf("%d\n", (A + B));
    printf("%d\n", (A - B));
    printf("%d\n", (A * B));
    printf("%d\n", (A / B));
    printf("%d\n", (A % B));
    X = (((A - B) * 10) + ((A + B) / 10));
    Y = (X + (X % 10));
    printf("%d\n", X);
    printf("%d\n", Y);
    
    return 0;
}
