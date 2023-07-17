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

byte* Encode(const byte* original, int length, int& comp_length) {
    assert(length != 0);

    comp_length = length < PIXEL_SIZE+COUNT_SIZE ? PIXEL_SIZE+COUNT_SIZE : length;
    byte* compressed = new byte[comp_length];
    int k = 0;

    const byte* pixel;
    const byte* next_pixel = original;
    int i = 0;
    while (i < length) {
        pixel = next_pixel;
        int count = 1;
        while (i < length) {
            i += PIXEL_SIZE;
            next_pixel = &original[i];
            bool eq = true;
            for (int j = 0; j < PIXEL_SIZE; ++j)
                if (next_pixel[j] != pixel[j]) {
                    eq = false;
                    break;
                }
            if (!eq) break;
            ++count;
        }
        int diff = comp_length - k;
        if (diff < PIXEL_SIZE+COUNT_SIZE) {
            comp_length = 2*comp_length;
            byte* n_buff = new byte[comp_length];
            memcpy(n_buff, compressed, k);
            delete[] compressed;
            compressed = n_buff;
        }
        compressed[k++] = count >> 8;
        compressed[k++] = count & 255;
        for (int j = 0; j < PIXEL_SIZE; ++j)
            compressed[k++] = pixel[j];
    }
    comp_length = k;
    return compressed;
};

byte* Decode(const byte* compressed, int length, int& orig_length) {
    assert(length >= PIXEL_SIZE + COUNT_SIZE);

    orig_length = 2*length;
    byte* original = new byte[orig_length];
    int k = 0;

    int i = 0;
    while (i < length) {
        const byte* current = &compressed[i];
        int count = 0;
        count += current[0] << 8;
        count += current[1];

        int diff = orig_length - k;
        if (diff < count*PIXEL_SIZE) {
            if (orig_length > PIXEL_SIZE*count - diff) orig_length *= 2;
            else orig_length = orig_length - diff + PIXEL_SIZE*count;
            byte* n_buff = new byte[orig_length];
            memcpy(n_buff, original, k);
            delete[] original;
            original = n_buff;
        }
        for (int j = 0; j < count; ++j) {
            for (int m = 0; m < PIXEL_SIZE; ++m)
                original[k++] = current[m+COUNT_SIZE];
        }
        i += PIXEL_SIZE+COUNT_SIZE;
    }
    orig_length = k;
    return original;
};
