#include <sstream>
#include <iostream>
#include "rle.hpp"
#include <time.h> 

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

int test(byte* data, int length, float& compr_) {
    byte_array compressed;
    byte_array decompressed;
    encode1(data, length, compressed);
    decode1(compressed.data(), compressed.size(), decompressed);

    compr_ = (float)compressed.size() / length;
    return length != decompressed.size(); 
}

int test_mp4(const std::string& filename_prefix, int frames, float& compr_,
double& encode_time, double& decode_time) {
    encode_time = decode_time = 0;
    vector<byte_array> compr_frames(frames);
    vector<int> orig_lengths(frames);
    vector<int> compr_lengths(frames);
    /////Encoding/////
    for (int i = 0; i < frames; ++i) {
        std::stringstream name;
        name << filename_prefix;
        if (i < 10) name << "00" << i;
        else if (i < 100) name << "0" << i;
        else name << i;
        name << ".jpg";
        int width, height, cnt;
        byte* data = stbi_load(name.str().c_str(), &width, &height, &cnt, 0);
        orig_lengths[i] = width*height*cnt; 
        byte_array compressed;
        clock_t start = clock();
            encode2(data, orig_lengths[i], compressed);
        clock_t end = clock();
        encode_time += (double)(end - start) / CLOCKS_PER_SEC;
        compr_frames[i] = compressed;
        compr_lengths[i] = compressed.size();
    }
    //////////////////
    /////Decoding/////

    float acc = 0;
    for (int i = 0; i < frames; ++i) {
        byte_array decompressed; 
        clock_t start = clock();
            decode2(compr_frames[i].data(), compr_lengths[i], decompressed);
        clock_t end = clock();
        decode_time += (double)(end - start) / CLOCKS_PER_SEC;
        if (decompressed.size() != orig_lengths[i]) {
            std::cout << "Error" << std::endl;
            return 1;
        }
        float compr_ =  (float)compr_lengths[i]/orig_lengths[i];
        // std::cout << compr_ << std::endl;
        acc += compr_;
    }

    //////////////////
    compr_ = acc/frames;
    return 0;
}

int main() {

    float compr_ = 0;
    double encode_time = 0;
    double decode_time = 0;
    if (!test_mp4("../imgs/NeuroMatrix_", 240, compr_, encode_time, decode_time)) 
    std::cout << "________________________\n" << compr_ << " | " << encode_time 
    << " | " << decode_time <<  std::endl;
    if (!test_mp4("../imgs/Map_", 133, compr_, encode_time, decode_time)) 
    std::cout << "________________________\n" << compr_ << " | " << encode_time 
    << " | " << decode_time <<  std::endl;

    return 0;
}
