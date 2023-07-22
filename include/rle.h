#include <stdio.h>

typedef unsigned char word;
typedef unsigned int uint;
const uint LIMIT_1B = 127;
const uint LENGTH_SIZE = 1;
const uint SYMBOL_SIZE = 1;

// Optimal(!) version with 1 word for len and negative lens;
int equal(const word *a, const word *b, uint count) {
    for (int i = 0; i < count; ++i) {
        if (a[i] != b[i]) return 1;
    }
    return 0;
}
void write_bytes(word *dest, const word *src, uint count) {
    for (int i = 0; i < count; ++i) dest[i] = src[i];
}

// Only positive LIMIT_1B
uint write_seq(word *dest, const word *b, uint count, uint size) {
    uint i = 0;
    for (; count > LIMIT_1B;
         dest += size, count -= LIMIT_1B, i += LENGTH_SIZE + SYMBOL_SIZE) {
        dest[0] = LIMIT_1B;
        dest += LENGTH_SIZE;
        write_bytes(dest, b, size);
    }
    dest[0] = count;
    dest += LENGTH_SIZE;
    write_bytes(dest, b, size);
    i += LENGTH_SIZE + SYMBOL_SIZE;
    return i;
}

uint write_nseq(word *dest, const word *src, uint count, uint size) {
    uint l = LIMIT_1B * size;
    uint i = count*size;
    for (; count > LIMIT_1B; count -= LIMIT_1B, i += LENGTH_SIZE) {
        dest[0] = -LIMIT_1B;
        dest += LENGTH_SIZE;
        write_bytes(dest, src, l);
        dest += l;
        src += l;
    }
    dest[0] = -count;
    dest += LENGTH_SIZE;
    write_bytes(dest, src, count * size);
    i += LENGTH_SIZE;
    return i;
}

void encode0(const word *original, int length, word *compressed) {
    int i = 1 * SYMBOL_SIZE, j = 0;
    int cnt1 = 1, cnt2 = 0;
    int begin = 0;
    for (; i < length; i += 1 * SYMBOL_SIZE, j += 1 * SYMBOL_SIZE, ++cnt1)
        if (equal(&original[j], &original[i], SYMBOL_SIZE)) {
            if (cnt1 == 1) {
                ++cnt2;
                cnt1 = 0;
                continue;
            }
            if (cnt2 != 0) {
                compressed +=
                    write_nseq(compressed, &original[begin], cnt2, SYMBOL_SIZE);
                cnt2 = 0;
            }
            begin = i;
            compressed +=
                write_seq(compressed, &original[j], cnt1, SYMBOL_SIZE);
            cnt1 = 0;
        }

    if (cnt1 == 1) {
        ++cnt2;
        compressed +=
            write_nseq(compressed, &original[begin], cnt2, SYMBOL_SIZE);
        return;
    }
    if (cnt2 != 0) {
        compressed +=
            write_nseq(compressed, &original[begin], cnt2, SYMBOL_SIZE);
    }
    compressed += write_seq(compressed, &original[j], cnt1, SYMBOL_SIZE);
    cnt1 = 0;
}

void decode0(const word *compressed, int length, word *original) {
    int i = 0;
    uint k = 0; 

    while (i < length) {
        const word *current = &compressed[i];
        char count = current[0];
        if (count > 0) {
            for (int j = 0; j < count; ++j) {
                write_bytes(&original[k], &current[LENGTH_SIZE], SYMBOL_SIZE);
                k += SYMBOL_SIZE;
            }
            i += LENGTH_SIZE + SYMBOL_SIZE;
            continue;
        }
        count *= -1;
        i += LENGTH_SIZE + SYMBOL_SIZE * count;

        write_bytes(&original[k], &current[LENGTH_SIZE], SYMBOL_SIZE * count);
        k += SYMBOL_SIZE * count;
    }
}
