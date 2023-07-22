#include "rle.h"
#include <stdio.h>
#define SIZE 400
word in[SIZE] __attribute__((section(".data.imu1")));
word out[SIZE] __attribute__((section(".data.imu2")));
word decomp[SIZE] __attribute__((section(".data.imu3")));

int main() {
    for (int i = 0; i < SIZE/4; ++i) {
        in[i] = 10;
        in[i+100] = 20;
        in[i+200] = 30;
        in[i+300] = 40+i;
    }

    encode0(in, SIZE, out);

    for (int i = 0; i < SIZE; ++i) {
        printf("%d ", out[i]);
    }
    printf("\n");
    decode0(out, SIZE, decomp);

    for (int i = 0; i < SIZE; ++i) {
        printf("%d ", decomp[i]);
    }
    printf("\n");
    for (int i = 0; i < SIZE; ++i) {
        if (in[i] != decomp[i]) printf("Error - %d\n", i);
    }
    return 0;
}
