#include <stdio.h>
#include <stdint.h>

int main() {
    int32_t A = 0;
    int32_t B = 0;
    int32_t I = 0;
    int32_t J = 0;
    int32_t X = 0;
    int32_t SQR = 0;

    scanf("%d", &A);
    scanf("%d", &B);
    for (I = A; I <= B; I++) {
        SQR = (I * I);
        printf("%d\n", SQR);
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
