#include "rle.hpp"

#include <cassert>

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
