#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using byte = unsigned char;
using byte_array = std::vector<byte>;
using std::vector;

// class IInputStream {
//    public:
//     virtual bool Read(byte &value) = 0;
// };

// class IOutputStream {
//    public:
//     virtual void Write(byte value) = 0;
// };

// class BInputStream : public IInputStream {
//    private:
//     byte_array &buffer;
//     int p;

//    public:
//     BInputStream(byte_array &_buffer) : buffer(_buffer), p(0){};

//     bool Read(byte &value) override {
//         if (p == buffer.size()) return false;

//         value = buffer[p++];
//         return true;
//     }
// };

// class BOutputStream : public IOutputStream {
//    private:
//     byte_array &buffer;

//    public:
//     BOutputStream(byte_array &_buffer) : buffer(_buffer){};

//     void Write(byte value) override { buffer.push_back(value); }

//     byte_array &get_buffer() { return buffer; }
// };

// class BStream : public IOutputStream, public IInputStream {
//    private:
//     byte_array &buffer;
//     int p;

//    public:
//     BStream(byte_array &_buffer) : buffer(_buffer), p(0){};

//     bool Read(byte &value) override {
//         if (p == buffer.size()) return false;

//         value = buffer[p++];
//         return true;
//     }

//     void Write(byte value) override { buffer.push_back(value); }

//     byte_array &get_buffer() { return buffer; }
// };

// class CBStream : public IOutputStream, public IInputStream {
//    private:
//     byte *buffer;
//     int size;
//     int cap;
//     int p;

//     void resize() {
//         int n_cap = 1;
//         if (cap != 0) n_cap = 2*cap;
//         byte * n_buffer = (byte*) malloc(n_cap);
//         memcpy(n_buffer, buffer, cap);
//         cap = n_cap;
//         free(buffer);
//         buffer = n_buffer;
//     }

//    public:
//     CBStream(byte *_buffer, int _size) : buffer(_buffer), p(0), size(_size), cap(_size){};
//     CBStream(int _size) : p(0), size(0), cap(_size){
//         buffer = (byte*) malloc(_size);
//     };

//     bool Read(byte &value) override {
//         if (p >= size) return false;
//         value = buffer[p++];
//         return true;
//     }

//     void Write(byte value) override {
//         if (size == cap) resize(); 
//         buffer[size] = value;
//         ++size;
//     }

//     const byte *get_buffer() const { return buffer; }
//     int get_size() const { return size; }

//     ~CBStream() {
//         free(buffer);
//     }
// };

// void Encode(IInputStream &original, IOutputStream &compressed);
// void Decode(IInputStream &compressed, IOutputStream &original);

// void Encode_p(IInputStream &original, IOutputStream &compressed);
// void Decode_p(IInputStream &compressed, IOutputStream &original);

// Optimal(!) version with 1 byte for len and negative lens;
bool equal(const byte *a, const byte *b, int count) {
    for (int i = 0; i < count; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}
void write_bytes(byte_array &dest, const byte *src, int count) {
    for (int i = 0; i < count; ++i) dest.push_back(src[i]);
}

// Only positive LIMIT_1B
template<char LIMIT_1B>
void write_seq(byte_array &dest, const byte *b, int count, int size) {
    while (count > LIMIT_1B) {
        dest.push_back(LIMIT_1B);
        write_bytes(dest, b, size);
        count -= LIMIT_1B;
    }
    dest.push_back(count);
    write_bytes(dest, b, size);
}

template<char LIMIT_1B>
void write_nseq(byte_array &dest, const byte *b, int count, int size) {
    while (count > LIMIT_1B) {
        dest.push_back(-LIMIT_1B);
        write_bytes(dest, b, LIMIT_1B * size);
        b += LIMIT_1B * size;
        count -= LIMIT_1B;
    }
    dest.push_back(-count);
    write_bytes(dest, b, count * size);
}

template<int SYMBOL_SIZE, char LIMIT_1B = 127>
void encode0(const byte *original, int length, byte_array &compressed) {
    int i = 1 * SYMBOL_SIZE, j = 0;
    int cnt1 = 1, cnt2 = 0;
    int begin = 0;
    for (; i < length; i += 1 * SYMBOL_SIZE, j += 1 * SYMBOL_SIZE, ++cnt1)
        if (!equal(&original[j], &original[i], SYMBOL_SIZE)) {
            if (cnt1 == 1) {
                ++cnt2;
                cnt1 = 0;
                continue;
            }
            if (cnt2 != 0) {
                write_nseq<LIMIT_1B>(compressed, &original[begin], cnt2, SYMBOL_SIZE);
                cnt2 = 0;
            }
            begin = i;
            write_seq<LIMIT_1B>(compressed, &original[j], cnt1, SYMBOL_SIZE);
            cnt1 = 0;
        }

    if (cnt1 == 1) {
        ++cnt2;
        write_nseq<LIMIT_1B>(compressed, &original[begin], cnt2, SYMBOL_SIZE);
        return;
    }
    if (cnt2 != 0) {
        write_nseq<LIMIT_1B>(compressed, &original[begin], cnt2, SYMBOL_SIZE);
    }
    write_seq<LIMIT_1B>(compressed, &original[j], cnt1, SYMBOL_SIZE);

    cnt1 = 0;
}
template<int SYMBOL_SIZE>
void decode0(const byte *compressed, int length, byte_array &original) {
    const int COUNT_SIZE = 1;
    int i = 0;

    while (i < length) {
        const byte *current = &compressed[i];
        char count = current[0];
        if (count > 0) {
            for (int j = 0; j < count; ++j)
                write_bytes(original, &current[COUNT_SIZE], SYMBOL_SIZE);
            i += COUNT_SIZE + SYMBOL_SIZE;
            continue;
        }
        count *= -1;
        i += COUNT_SIZE + SYMBOL_SIZE * count;

        write_bytes(original, &current[COUNT_SIZE], SYMBOL_SIZE * count);
    }
}

// Simple version with 1 byte for len and no negative lens;
template<int SYMBOL_SIZE, char LIMIT_1B = 127>
void encode1(const byte *original, int length, byte_array &compressed) {
    int i = 1 * SYMBOL_SIZE, j = 0;
    int cnt1 = 1;
    for (; i < length; i += 1 * SYMBOL_SIZE, j += 1 * SYMBOL_SIZE, ++cnt1)
        if (!equal(&original[j], &original[i], SYMBOL_SIZE)) {
            write_seq<LIMIT_1B>(compressed, &original[j], cnt1, SYMBOL_SIZE);
            cnt1 = 0;
        }
    write_seq<LIMIT_1B>(compressed, &original[j], cnt1, SYMBOL_SIZE);
};
template<int SYMBOL_SIZE>
void decode1(const byte *compressed, int length, byte_array &original) {
    const int COUNT_SIZE = 1;
    for (int i = 0; i < length; i += COUNT_SIZE + SYMBOL_SIZE) {
        const byte *current = &compressed[i];
        char count = current[0];
        for (int j = 0; j < count; ++j)
            write_bytes(original, &current[COUNT_SIZE], SYMBOL_SIZE);
        continue;
    }
};


// Version with 2 bytes for len and negative lens in direct code;
//Only positive LIMIT_2B
template<short LIMIT_2B>
void write_seq_2bytes(byte_array &dest, const byte *b, int count, int size) {
    while (count > LIMIT_2B) {
        dest.push_back(127);
        dest.push_back(255);
        write_bytes(dest, b, size);
        count -= LIMIT_2B;
    }
    dest.push_back(count>>8);
    dest.push_back(count&255);
    write_bytes(dest, b, size);
}
template<short LIMIT_2B>
void write_nseq_2bytes(byte_array &dest, const byte *b, int count, int size) {
    while (count > LIMIT_2B) {
        dest.push_back(255);
        dest.push_back(255);
        write_bytes(dest, b, LIMIT_2B * size);
        b += LIMIT_2B * size;
        count -= LIMIT_2B;
    }
    dest.push_back((count>>8)+128);
    dest.push_back(count&255);
    write_bytes(dest, b, count * size);
}

template<int SYMBOL_SIZE, short LIMIT_2B = 32'767>
void encode2(const byte *original, int length, byte_array &compressed) {
    int i = 1 * SYMBOL_SIZE, j = 0;
    int cnt1 = 1, cnt2 = 0;
    int begin = 0;
    for (; i < length; i += 1 * SYMBOL_SIZE, j += 1 * SYMBOL_SIZE, ++cnt1)
        if (!equal(&original[j], &original[i], SYMBOL_SIZE)) {
            if (cnt1 == 1) {
                ++cnt2;
                cnt1 = 0;
                continue;
            }
            if (cnt2 != 0) {
                write_nseq_2bytes<LIMIT_2B>(compressed, &original[begin], cnt2, SYMBOL_SIZE);
                cnt2 = 0;
            }
            begin = i;
            write_seq_2bytes<LIMIT_2B>(compressed, &original[j], cnt1, SYMBOL_SIZE);
            cnt1 = 0;
        }

    if (cnt1 == 1) {
        ++cnt2;
        write_nseq_2bytes<LIMIT_2B>(compressed, &original[begin], cnt2, SYMBOL_SIZE);
        return;
    }
    if (cnt2 != 0) {
        write_nseq_2bytes<LIMIT_2B>(compressed, &original[begin], cnt2, SYMBOL_SIZE);
    }
    write_seq_2bytes<LIMIT_2B>(compressed, &original[j], cnt1, SYMBOL_SIZE);

    cnt1 = 0;
}
template<int SYMBOL_SIZE>
void decode2(const byte *compressed, int length, byte_array &original) {
    const int COUNT_SIZE = 2;
    int i = 0;

    while (i < length) {
        const byte *current = &compressed[i];
        int count = current[0];
        if (count < 128) {
            count <<= 8;
            count += current[1];
            for (int j = 0; j < count; ++j)
                write_bytes(original, &current[COUNT_SIZE], SYMBOL_SIZE);
            i += COUNT_SIZE + SYMBOL_SIZE;
            continue;
        }
        count -= 128;
        count <<= 8;
        count += current[1];
        i += COUNT_SIZE + SYMBOL_SIZE * count;

        write_bytes(original, &current[COUNT_SIZE], SYMBOL_SIZE * count);
    }
}
