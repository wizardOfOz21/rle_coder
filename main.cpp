#include "rle.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

template <class T>
void reverse(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n / 2; ++i) {
        std::swap(arr[i], arr[n - 1 - i]);
    }
}

int main() {
    std::string str = "../11.png";
    int width, height, cnt;
    byte* data = stbi_load(str.c_str(), &width, &height, &cnt, 0);
    // byte_array original(data, data+width*height*cnt);

    // original.push_back(0);
    // original.push_back(0);
    // original.push_back(0);
    // original.push_back(0);
    
    std::cout << width*height*cnt << std::endl;

    // byte_array original{1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0,
    //                     3, 4, 5, 0, 3, 4, 5, 0, 0, 0, 0, 0};

    // byte_array compressed;
    // byte_array res;
    // BStream in(original);
    // BStream out(compressed);
    // BStream t(res);
    CBStream in(data, width*height*cnt);
    CBStream out(0);
    CBStream t(0);
    in.Write(0);
    in.Write(0);
    in.Write(0);
    in.Write(0);
    Encode_p(in, out);
    // std::cout << out.get_buffer().size() << std::endl;
    std::cout << out.get_size() << std::endl;
    Decode_p(out, t);
    // std::cout << t.get_buffer().size() << std::endl;
    std::cout << t.get_size() << std::endl;

    // stbi_write_png("../copy.png", width, height,
    //                          cnt, t.get_buffer(), width * cnt);
    return 0;
}
