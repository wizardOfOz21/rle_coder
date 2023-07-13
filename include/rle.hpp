#include <iostream>
#include <string>
#include <vector>

using byte = unsigned char;
using byte_array = std::vector<byte>;

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
    BStream(byte_array &_buffer) : buffer(_buffer), p(0) {};

    bool Read(byte &value) override {
        if (p == buffer.size()) return false;

        value = buffer[p++];
        return true;
    }
    
    void Write(byte value) override { buffer.push_back(value); }

    byte_array &get_buffer() { return buffer; }
};

void Encode(IInputStream &original, IOutputStream &compressed);

void Decode(IInputStream &compressed, IOutputStream &original);
