typedef unsigned char word;
typedef unsigned int uint;
const uint LIMIT_1B = 127;
const uint LENGTH_SIZE = 1;
const uint SYMBOL_SIZE = 3;

int equal(const word *a, const word *b, uint count);
void write_bytes(word *dest, const word *src, uint count);

void encode0(const word *original, int length, word *compressed);
void decode0(const word *compressed, int length, word *original);
