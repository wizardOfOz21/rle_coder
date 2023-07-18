#include "rle.hpp"

#include <cassert>

const int COUNT_SIZE = 2;
const int PIXEL_SIZE = 3;
const byte_array ESCAPE_PIXEL = byte_array{0, 0, 0, 0};

void Encode(IInputStream &original, IOutputStream &compressed) {
    byte current = 0;
    original.Read(current);
    while (current != 0) {
        int count = 1;
        byte value = current;
        while (true) {
            byte b = 0;
            original.Read(b);
            if (b != current) {
                current = b;
                break;
            };
            ++count;
        }
        compressed.Write(count);
        compressed.Write(value);
    }
    compressed.Write(0);
};

void Decode(IInputStream &compressed, IOutputStream &original) {
    byte count = 0;
    compressed.Read(count);
    while (count != 0) {
        byte value = 0;
        compressed.Read(value);
        assert(value != 0);
        for (int i = 0; i < count; ++i) {
            original.Write(value);
        }
        compressed.Read(count);
    }
    original.Write(0);
};

bool read_pixel(IInputStream &src, byte_array &res) {
    for (int i = 0; i < PIXEL_SIZE; ++i) {
        if (!src.Read(res[i])) return false;
    }
    return true;
}

void write_pixel(IOutputStream &buff, const byte_array &data) {
    for (int i = 0; i < PIXEL_SIZE; ++i) {
        buff.Write(data[i]);
    }
}

void Encode_p(IInputStream &original, IOutputStream &compressed) {
    byte_array current(PIXEL_SIZE);
    read_pixel(original, current);
    while (current != ESCAPE_PIXEL) {
        int count = 1;
        byte_array value(current);
        while (true) {
            byte_array b(PIXEL_SIZE);
            read_pixel(original, b);
            if (b != current) {
                current = b;
                break;
            };
            ++count;
        }
        compressed.Write(count >> 8);
        compressed.Write(count & 255);
        write_pixel(compressed, value);
    }
    compressed.Write(0);
};

bool read_int(IInputStream &src, int &res, int count) {
    res = 0;
    for (int i = 0; i < count; ++i) {
        byte b = 0;
        if (!src.Read(b)) return false;
        res += b << 8 * (count - i - 1);
    }
    return true;
}

void Decode_p(IInputStream &compressed, IOutputStream &original) {
    int count = 0;
    read_int(compressed, count, 2);
    while (count != 0) {
        byte_array value(PIXEL_SIZE);
        read_pixel(compressed, value);
        assert(value != ESCAPE_PIXEL);
        for (int i = 0; i < count; ++i) {
            write_pixel(original, value);
        }
        read_int(compressed, count, 2);
    }
    write_pixel(original, ESCAPE_PIXEL);
};



// Optimal(?) version with 1 byte for len and negative lens;
const int LIMIT = 127;
bool equal(const byte *a, const byte *b, int count) {
    for (int i = 0; i < count; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

void write(byte_array &dest, const byte *src, int count) {
    for (int i = 0; i < count; ++i) dest.push_back(src[i]);
}

void write_seq(byte_array &dest, const byte *b, int count, int size) {
    while (count > LIMIT) {
        dest.push_back(LIMIT);
        write(dest, b, size);
        count -= LIMIT;
    }
    dest.push_back(count);
    write(dest, b, size);
}

void write_nseq(byte_array &dest, const byte *b, int count, int size) {
    while (count > LIMIT) {
        dest.push_back(-LIMIT);
        write(dest, b, LIMIT * size);
        b += LIMIT;
        count -= LIMIT;
    }
    dest.push_back(-count);
    write(dest, b, count * size);
}

void encode0(const byte *original, int length, byte_array &compressed) {
    const int PIXEL_SIZE = 1;
    int i = 1 * PIXEL_SIZE, j = 0;
    int cnt1 = 1, cnt2 = 0;
    int begin = 0;
    for (; i < length; i += 1 * PIXEL_SIZE, j += 1 * PIXEL_SIZE, ++cnt1)
        if (!equal(&original[j], &original[i], PIXEL_SIZE)) {
            if (cnt1 == 1) {
                ++cnt2;
                cnt1 = 0;
                continue;
            }
            if (cnt2 != 0) {
                write_nseq(compressed, &original[begin], cnt2, PIXEL_SIZE);
                cnt2 = 0;
            }
            begin = i;
            write_seq(compressed, &original[j], cnt1, PIXEL_SIZE);
            cnt1 = 0;
        }

    if (cnt1 == 1) {
        ++cnt2;
        write_nseq(compressed, &original[begin], cnt2, PIXEL_SIZE);
        return;
    }
    if (cnt2 != 0) {
        write_nseq(compressed, &original[begin], cnt2, PIXEL_SIZE);
    }
    write_seq(compressed, &original[j], cnt1, PIXEL_SIZE);

    cnt1 = 0;
}

void decode0(const byte *compressed, int length, byte_array &original) {
    const int COUNT_SIZE = 1;
    const int PIXEL_SIZE = 1;
    int i = 0;

    while (i < length) {
        const byte *current = &compressed[i];
        char count = current[0];
        if (count > 0) {
            for (int j = 0; j < count; ++j)
                write(original, &current[COUNT_SIZE], PIXEL_SIZE);
            i += COUNT_SIZE + PIXEL_SIZE;
            continue;
        }
        count *= -1;
        i += COUNT_SIZE + PIXEL_SIZE * count;

        write(original, &current[COUNT_SIZE], PIXEL_SIZE * count);
    }
}


// Simple version with 1 byte for len and no negative lens;
void encode1(const byte *original, int length, byte_array &compressed) {
    const int PIXEL_SIZE = 1;
    int i = 1 * PIXEL_SIZE, j = 0;
    int cnt1 = 1;
    for (; i < length; i += 1 * PIXEL_SIZE, j += 1 * PIXEL_SIZE, ++cnt1)
        if (!equal(&original[j], &original[i], PIXEL_SIZE)) {
            write_seq(compressed, &original[j], cnt1, PIXEL_SIZE);
            cnt1 = 0;
        }
    write_seq(compressed, &original[j], cnt1, PIXEL_SIZE);
};

void decode1(const byte *compressed, int length, byte_array &original) {
    const int COUNT_SIZE = 1;
    const int PIXEL_SIZE = 1;
    for (int i = 0; i < length; i += COUNT_SIZE + PIXEL_SIZE) {
        const byte *current = &compressed[i];
        char count = current[0];
        for (int j = 0; j < count; ++j)
            write(original, &current[COUNT_SIZE], PIXEL_SIZE);
        continue;
    }
};



// Version with 2 bytes for len and negative lens in direct code;
const int LIMIT_2B = 32'767;
void write_seq_2bytes(byte_array &dest, const byte *b, int count, int size) {
    while (count > LIMIT_2B) {
        dest.push_back(127);
        dest.push_back(255);
        write(dest, b, size);
        count -= LIMIT_2B;
    }
    dest.push_back(count>>8);
    dest.push_back(count&255);
    write(dest, b, size);
}

void write_nseq_2bytes(byte_array &dest, const byte *b, int count, int size) {
    while (count > LIMIT_2B) {
        dest.push_back(255);
        dest.push_back(255);
        write(dest, b, LIMIT_2B * size);
        b += LIMIT_2B;
        count -= LIMIT_2B;
    }
    dest.push_back((count>>8)+128);
    dest.push_back(count&255);
    write(dest, b, count * size);
}

void encode2(const byte *original, int length, byte_array &compressed) {
    const int PIXEL_SIZE = 1;
    int i = 1 * PIXEL_SIZE, j = 0;
    int cnt1 = 1, cnt2 = 0;
    int begin = 0;
    for (; i < length; i += 1 * PIXEL_SIZE, j += 1 * PIXEL_SIZE, ++cnt1)
        if (!equal(&original[j], &original[i], PIXEL_SIZE)) {
            if (cnt1 == 1) {
                ++cnt2;
                cnt1 = 0;
                continue;
            }
            if (cnt2 != 0) {
                write_nseq_2bytes(compressed, &original[begin], cnt2, PIXEL_SIZE);
                cnt2 = 0;
            }
            begin = i;
            write_seq_2bytes(compressed, &original[j], cnt1, PIXEL_SIZE);
            cnt1 = 0;
        }

    if (cnt1 == 1) {
        ++cnt2;
        write_nseq_2bytes(compressed, &original[begin], cnt2, PIXEL_SIZE);
        return;
    }
    if (cnt2 != 0) {
        write_nseq_2bytes(compressed, &original[begin], cnt2, PIXEL_SIZE);
    }
    write_seq_2bytes(compressed, &original[j], cnt1, PIXEL_SIZE);

    cnt1 = 0;
}

void decode2(const byte *compressed, int length, byte_array &original) {
    const int COUNT_SIZE = 2;
    const int PIXEL_SIZE = 1;
    int i = 0;

    while (i < length) {
        const byte *current = &compressed[i];
        int count = current[0];
        if (count < 128) {
            count += current[1];
            for (int j = 0; j < count; ++j)
                write(original, &current[COUNT_SIZE], PIXEL_SIZE);
            i += COUNT_SIZE + PIXEL_SIZE;
            continue;
        }
        count -= 128;
        count <<= 8;
        count += current[1];
        i += COUNT_SIZE + PIXEL_SIZE * count;

        write(original, &current[COUNT_SIZE], PIXEL_SIZE * count);
    }
}
