#include "rle.h"
#include <stdio.h>
#define SIZE 400
word in[SIZE] __attribute__((section(".data.imu1")));
word out[SIZE] __attribute__((section(".data.imu2")));
word decomp[SIZE] __attribute__((section(".data.imu3")));

// extern "C" int Neg ( unsigned int *value );
int main() {
    for (int i = 0; i < SIZE/4; ++i) {
        in[i] = 10;
        in[i+100] = 20;
        in[i+200] = 30;
        in[i+300] = 40+i;
    }

    // printf("%d\n", equal(in, &in[100], 3));
    // write_bytes(out, in, 100);
    // write_seq(out, in, 255, 3);
    // printf("%d\n",write_nseq(out, &in[100], 254, 1));
    // write_bytes(out, &in[300], 100);

    encode0(in, SIZE, out);

    for (int i = 0; i < SIZE; ++i) {
        printf("%d", out[i]);
    }

    decode0(out, SIZE, decomp);

    for (int i = 0; i < SIZE; ++i) {
        if (in[i] != decomp[i]) printf("Error - %d\n", i);
    }
}
