#include <stdio.h>
#include <stdint.h>

int main() {
    int32_t A = 0;
    int32_t B = 0;
    int32_t C = 0;
    int32_t MAX = 0;

    scanf("%d", &A);
    scanf("%d", &B);
    scanf("%d", &C);
    MAX = A;
    if ((B >= MAX)) goto CHECKB;
    goto CHECKC;
CHECKB:;
    MAX = B;
CHECKC:;
    if ((C >= MAX)) goto SETC;
    goto PRINTMX;
SETC:;
    MAX = C;
PRINTMX:;
    printf("%d\n", MAX);
    if ((((A == B) && (A == C)) && (B == C))) goto ALLEQ;
    printf("%d\n", 0);
    goto CHKNEG;
ALLEQ:;
    printf("%d\n", 1);
CHKNEG:;
    if ((((A <= (0 - 1)) || (B <= (0 - 1))) || (C <= (0 - 1)))) goto ISNEG;
    printf("%d\n", 0);
    goto CHKNOT;
ISNEG:;
    printf("%d\n", (0 - 1));
CHKNOT:;
    if ((A >= (B + C))) goto NOTLESS;
    printf("%d\n", 0);
    goto DONE;
NOTLESS:;
    printf("%d\n", 10);
DONE:;

    return 0;
}
