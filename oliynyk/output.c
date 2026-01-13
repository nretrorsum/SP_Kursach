/**
 * Generated code from O16 language
 * Variant: Oliynyk Bohdan Romanovych
 */

#include <stdio.h>
#include <stdint.h>

int main() {
    // Variable declarations (int_2 = int16_t)
    int16_t var_I = 0;
    int16_t var_SUM = 0;
    int16_t var_N = 0;
    int16_t var_F = 0;
    
    scanf("%hd", &var_N);
    var_I = 1;
    var_SUM = 0;
    while ((var_I < (var_N + 1))) {
        var_SUM = (var_SUM + var_I);
        var_I = (var_I + 1);
    }
    printf("%d\n", var_SUM);
    scanf("%hd", &var_N);
    var_F = 1;
    var_I = 1;
    while ((var_I < (var_N + 1))) {
        var_F = (var_F * var_I);
        var_I = (var_I + 1);
    }
    printf("%d\n", var_F);
    var_I = 5;
    while ((var_I > 0)) {
        printf("%d\n", var_I);
        var_I = (var_I - 1);
    }
    return 0;
}
