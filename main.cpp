#include "rle.hpp"

template<class T>
void reverse(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n/2; ++i) {
        std::swap(arr[i], arr[n-1-i]);
    }
}

int main()
{
    byte_array original{'a','a','a','b','b','b','c','c','c','c','d','\0'};
    byte_array compressed;
    BStream in(original);
    BStream out(compressed);
    Encode(in,out);
    Decode(out,in);
    return 0;
}
