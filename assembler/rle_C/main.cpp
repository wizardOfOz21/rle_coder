#include <stdio.h>
#include <stdlib.h>

#include "rle.h"
#define SIZE 400
word in[SIZE] __attribute__((section(".data.imu1")));
word out[SIZE] __attribute__((section(".data.imu2")));
word decomp[SIZE] __attribute__((section(".data.imu3")));

extern "C" int Encode(word* src, word* dest, uint count);
extern "C" int Decode(word* src, word* dest, uint count);
const int N = 7;
const int M = 100;
int main() {
    char tests[N][M] = {"a", "aaa", "bbcdfaaab", "aaabbbb", "abbba", "aaacdf", "abcdefg"};
    int test_size[N] = {1, 3, 9, 7, 5, 6, 7};
    for (int i = 0; i < N; ++i) {
        int size = test_size[i];
        char* test = tests[i];
        char comp[M] = {0};
        int k = Encode((word*)test, (word*)comp, size);
        char decomp[M] = {0};
        Decode((word*)comp, (word*)decomp, k);
        printf("test %d: \"%s\" -> \"%s\": correct\n", i, comp, decomp);
        for (int j = 0; j < size; ++j) {
            if (test[j] != decomp[j]) {
                printf("test %d: Error in \"%s\" - %d\n", i, test, j);
            };
        }
    }

    for (int i = 0; i < 100; ++i) {
        int size = rand() % 1000 + 1;
        int* arr = (int*)malloc(size);
        for (int j = 0; j < size; ++j) arr[j] = rand() % 100;
        int* compr = (int*)malloc(size*2);
        int compr_size = Encode((word*)arr, (word*)compr, size);
        if (compr_size > size*2) printf("!!!!!Problem!!!!!!");
        int* decomp = (int*)malloc(size);
        Decode((word*)compr, (word*)decomp, compr_size);
        for (int j = 0; j < size; ++j) {
            if (arr[j] != decomp[j]) {
                printf("test %d: Error in %d\n", i, j);
            };
        }
        free(arr);
        free(compr);
        free(decomp);
    }

    for (int i = 0; i < 100; ++i) {
        int size = rand() % 1000 + 1;
        int* arr = (int*)malloc(size);
        for (int j = 0; j < size; ++j) arr[j] = rand() % 2;
        int* compr = (int*)malloc(size*2);
        int compr_size = Encode((word*)arr, (word*)compr, size);
        if (compr_size > size*2) printf("!!!!!Problem!!!!!!");
        int* decomp = (int*)malloc(size);
        Decode((word*)compr, (word*)decomp, compr_size);
        for (int j = 0; j < size; ++j) {
            if (arr[j] != decomp[j]) {
                printf("test %d: Error in %d\n", i, j);
            };
        }
        free(arr);
        free(compr);
        free(decomp);
    }
}
