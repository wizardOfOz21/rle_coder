#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "maxtimer.h"

#include "rle.h"
#define SIZE 10000
word A[SIZE] __attribute__((section(".data.imu1")));
word B[SIZE] __attribute__((section(".data.imu2")));

extern "C" int Encode(word* src, word* dest, uint count);
extern "C" int Encode1(word* src, word* dest, uint count);
extern "C" int Decode(word* src, word* dest, uint count);
const int N = 9;
const int M = 100;
char tests[N][M] = {"aa", "aaa", "bbcdfaaab", "aaabbbb", "abbba", "aaacdf", "abcdefg", "gbb", "aabbab"};
const int test_size[N] = {2, 3, 9, 7, 5, 6, 7, 3, 6};
int main() {
    // DISABLE_SYS_TIMER()
    clock_t time = 0;
    for (int i = 0; i < N; ++i) {
        int size = test_size[i];
        char* test = tests[i];
        char comp[M] = {0};
        clock_t begin = clock();
        int k = Encode1((word*)test, (word*)comp, size);
        clock_t end = clock();
        time += end - begin;
        char decomp[M] = {0};
        Decode((word*)comp, (word*)decomp, k);
        for (int j = 0; j < size; ++j) {
            if (test[j] != decomp[j]) {
                printf("test %d: \"%s\" -> \"%s\" -> \"%s\": ERROR\n", i, test, comp, decomp);
            };
        }
        printf("test %d: \"%s\"(%d) -> \"%s\": correct\n", i, comp, k, decomp);
    }

    int count = 0;
    for (int i = 0; i < 100; ++i) {
        int size = rand() % 1000 + 1;
        count += size;
        int* arr = (int*)malloc(size);
        for (int j = 0; j < size; ++j) arr[j] = rand() % 100;
        int* compr = (int*)malloc(size*2);
        clock_t begin = clock();
        int compr_size = Encode1((word*)arr, (word*)compr, size);
        clock_t end = clock();
        time += end - begin;
        if (compr_size > size*2) printf("!!!!!Problem!!!!!!");
        int* decomp = (int*)malloc(size);
        Decode((word*)compr, (word*)decomp, compr_size);
        for (int j = 0; j < size; ++j) {
            if (arr[j] != decomp[j]) {
                printf("test %d: Error in %d:\n", i, j);
                printf("arr:");
                for (int k = 0; k < size; ++k) {
                    if (k == j) {
                        printf("  %d   ", arr[k]);
                        continue;
                    }
                    printf("%d ", arr[k]);
                }
                printf("\ncompr:");
                for (int k = 0; k < compr_size; ++k) {
                    printf("%d ", compr[k]);
                }
                printf("\ndecompr:");
                for (int k = 0; k < size; ++k) {
                    if (k == j) {
                        printf("  %d   ", decomp[k]);
                        continue;
                    }
                    printf("%d ", decomp[k]);
                }
                printf("\n");
                break;
            };
        }
        free(arr);
        free(compr);
        free(decomp);
    }

    for (int i = 0; i < 100; ++i) {
        int size = rand() % 1000 + 1;
        count += size;
        int* arr = (int*)malloc(size);
        for (int j = 0; j < size; ++j) arr[j] = rand() % 2;
        int* compr = (int*)malloc(size*2);
        clock_t begin = clock();
        int compr_size = Encode1((word*)arr, (word*)compr, size);
        clock_t end = clock();
        time += end - begin;
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

    // count += SIZE;
    // for (int i = 0; i < SIZE; ++i) {
    //     A[i] = 0;
    // }
    // clock_t begin = clock();
    // Encode1((word*)A, (word*)B, SIZE);
    // clock_t end = clock();
    // time += end - begin;

    // count += SIZE;
    // for (int i = 0; i < SIZE; ++i) {
    //     A[i] = i%2;
    // }
    // clock_t begin = clock();
    // Encode1((word*)A, (word*)B, SIZE);
    // clock_t end = clock();
    // time += end - begin;

    printf("%lld\n", time);
    printf("%d\n", count);
}
