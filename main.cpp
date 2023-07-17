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
    int decomp_length;
    int comp_length;
    byte* compressed = Encode(data, length, comp_length);
    byte* original = Decode(compressed, comp_length, decomp_length);

    compr_ = (float)comp_length / length;
    return length != decomp_length; 
}

int test_mp4(const std::string& filename_prefix, int frames, float& compr_,
double& encode_time, double& decode_time) {
    encode_time = decode_time = 0;
    byte** compr_frames = new byte*[frames];
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
        int comp_length;
        clock_t start = clock();
            byte* compressed = Encode(data, orig_lengths[i], comp_length);
        clock_t end = clock();
        encode_time += (double)(end - start) / CLOCKS_PER_SEC;
        compr_frames[i] = compressed;
        compr_lengths[i] = comp_length;
    }
    //////////////////
    /////Decoding/////

    float acc = 0;
    for (int i = 0; i < frames; ++i) {
        int decomp_length;
        clock_t start = clock();
            byte* decompressed = Decode(compr_frames[i], compr_lengths[i], decomp_length);
        clock_t end = clock();
        decode_time += (double)(end - start) / CLOCKS_PER_SEC;
        if (decomp_length != orig_lengths[i]) {
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
