#include "rle.hpp"

#include <cassert>

const int PIXEL_SIZE = 4;
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
        compressed.Write(count);
        write_pixel(compressed, value);
    }
    compressed.Write(0);
};

void Decode_p(IInputStream &compressed, IOutputStream &original) {
    byte count = 0;
    compressed.Read(count);
    while (count != 0) {
        byte_array value(PIXEL_SIZE);
        read_pixel(compressed, value);
        assert(value != ESCAPE_PIXEL);
        for (int i = 0; i < count; ++i) {
            write_pixel(original, value);
        }
        compressed.Read(count);
    }
    write_pixel(original, ESCAPE_PIXEL);
};
