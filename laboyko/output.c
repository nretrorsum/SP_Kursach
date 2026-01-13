#include <stdio.h>
#include <stdint.h>

int main() {
    int32_t A = 0;
    int32_t B = 0;
    int32_t X = 0;
    int32_t Y = 0;
    int32_t Z = 0;

    scanf("%d", &A);
    scanf("%d", &B);
    Z = (A + B);
    printf("%d\n", Z);
    Z = (A - B);
    printf("%d\n", Z);
    Z = (A * B);
    printf("%d\n", Z);
    Z = (A / B);
    printf("%d\n", Z);
    Z = (A % B);
    printf("%d\n", Z);
    X = (((A - B) * 10) + ((A + B) / 10));
    Y = (X + (X % 10));
    printf("%d\n", X);
    printf("%d\n", Y);

    return 0;
}
