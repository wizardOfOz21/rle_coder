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
    byte_array original{1,1,1,0,1,1,1,0,1,1,1,0,3,4,5,0,3,4,5,0,0,0,0,0};
    byte_array compressed;
    BStream in(original);
    BStream out(compressed);
    Encode_p(in,out);
    Decode_p(out,in);
    return 0;
}
