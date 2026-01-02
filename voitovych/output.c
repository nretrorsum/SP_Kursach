/**
 * Generated code from V07 language
 * Program: DM
 * Variant: Voitovych Oleksandr Viktorovych
 */

#include <stdio.h>
#include <stdint.h>

int main() {
    int16_t A, B, X;
    
    scanf("%hd", &A);
    scanf("%hd", &B);
    X = (A + B);
    printf("%d\n", X);
    if ((A > B)) goto LA;
    printf("%d\n", B);
    goto LB;
LA:
    printf("%d\n", A);
LB:
    for (X = 1; X <= 5; X++) {
        printf("%d\n", (X * X));
    }
    
    return 0;
}
