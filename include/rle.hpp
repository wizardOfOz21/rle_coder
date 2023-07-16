#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using byte = unsigned char;
using byte_array = std::vector<byte>;
using std::vector;

class IInputStream {
   public:
    virtual bool Read(byte &value) = 0;
};

class IOutputStream {
   public:
    virtual void Write(byte value) = 0;
};

class BInputStream : public IInputStream {
   private:
    byte_array &buffer;
    int p;

   public:
    BInputStream(byte_array &_buffer) : buffer(_buffer), p(0){};

    bool Read(byte &value) override {
        if (p == buffer.size()) return false;

        value = buffer[p++];
        return true;
    }
};

class BOutputStream : public IOutputStream {
   private:
    byte_array &buffer;

   public:
    BOutputStream(byte_array &_buffer) : buffer(_buffer){};

    void Write(byte value) override { buffer.push_back(value); }

    byte_array &get_buffer() { return buffer; }
};

class BStream : public IOutputStream, public IInputStream {
   private:
    byte_array &buffer;
    int p;

   public:
    BStream(byte_array &_buffer) : buffer(_buffer), p(0){};

    bool Read(byte &value) override {
        if (p == buffer.size()) return false;

        value = buffer[p++];
        return true;
    }

    void Write(byte value) override { buffer.push_back(value); }

    byte_array &get_buffer() { return buffer; }
};

class CBStream : public IOutputStream, public IInputStream {
   private:
    byte *buffer;
    int size;
    int cap;
    int p;

    void resize() {
        int n_cap = 1;
        if (cap != 0) n_cap = 2*cap;
        byte * n_buffer = (byte*) malloc(n_cap);
        memcpy(n_buffer, buffer, cap);
        cap = n_cap;
        free(buffer);
        buffer = n_buffer;
    }

   public:
    CBStream(byte *_buffer, int _size) : buffer(_buffer), p(0), size(_size), cap(_size){};
    CBStream(int _size) : p(0), size(0), cap(_size){
        buffer = (byte*) malloc(_size);
    };

    bool Read(byte &value) override {
        if (p >= size) return false;
        value = buffer[p++];
        return true;
    }

    void Write(byte value) override {
        if (size == cap) resize(); 
        buffer[size] = value;
        ++size;
    }

    const byte *get_buffer() const { return buffer; }
    int get_size() const { return size; }

    ~CBStream() {
        free(buffer);
    }
};

void Encode(IInputStream &original, IOutputStream &compressed);
void Decode(IInputStream &compressed, IOutputStream &original);

void Encode_p(IInputStream &original, IOutputStream &compressed);
void Decode_p(IInputStream &compressed, IOutputStream &original);
