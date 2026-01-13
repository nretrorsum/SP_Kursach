/**
 * Generated code from O16 language
 * Variant: Oliynyk Bohdan Romanovych
 */

#include <stdio.h>
#include <stdint.h>

int main() {
    // Variable declarations (int_2 = int16_t)
    int16_t var_A = 0;
    int16_t var_B = 0;
    int16_t var_I = 0;
    int16_t var_J = 0;
    int16_t var_X = 0;
    int16_t var_SQR = 0;
    
    scanf("%hd", &var_A);
    scanf("%hd", &var_B);
    var_I = var_A;
    while ((var_I < (var_B + 1))) {
        var_SQR = (var_I * var_I);
        printf("%d\n", var_SQR);
        var_I = (var_I + 1);
    }
    var_X = 0;
    var_I = 1;
    while ((var_I < (var_A + 1))) {
        var_J = 1;
        while ((var_J < (var_B + 1))) {
            var_X = (var_X + 1);
            var_J = (var_J + 1);
        }
        var_I = (var_I + 1);
    }
    printf("%d\n", var_X);
    return 0;
}
