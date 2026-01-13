/**
 * Generated code from V03 language
 * Program: BR
 * Variant: Voitovych Oleksandr Viktorovych
 */

#include <stdio.h>
#include <stdint.h>

int main() {
    int16_t A, B, C, MX;
    
    scanf("%hd", &A);
    scanf("%hd", &B);
    scanf("%hd", &C);
    MX = A;
    if ((B > MX)) goto LA;
    goto LB;
LA:
    MX = B;
LB:
    if ((C > MX)) goto LC;
    goto LD;
LC:
    MX = C;
LD:
    printf("%d\n", MX);
    if ((((A == B) && (A == C)) && (B == C))) goto LE;
    printf("%d\n", 0);
    goto LF;
LE:
    printf("%d\n", 1);
LF:
    if ((((A < 0) || (B < 0)) || (C < 0))) goto LG;
    printf("%d\n", 0);
    goto LH;
LG:
    printf("%d\n", (0 - 1));
LH:
    if ((!(A < (B + C)))) goto LI;
    printf("%d\n", 0);
    goto LJ;
LI:
    printf("%d\n", 10);
LJ:
    A = 0;
    
    return 0;
}
