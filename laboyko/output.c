#include <stdio.h>
#include <stdint.h>

int main() {
    int32_t I = 0;
    int32_t SUM = 0;
    int32_t N = 0;
    int32_t FACT = 0;

    scanf("%d", &N);
    SUM = 0;
    for (I = 1; I <= N; I++) {
        SUM = (SUM + I);
    }
    printf("%d\n", SUM);
    scanf("%d", &N);
    FACT = 1;
    for (I = 1; I <= N; I++) {
        FACT = (FACT * I);
    }
    printf("%d\n", FACT);
    I = 5;
LOOP:;
    printf("%d\n", I);
    I = (I - 1);
    if ((I >= 1)) goto LOOP;

    return 0;
}
