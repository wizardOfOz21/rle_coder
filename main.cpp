#include <time.h>

#include <fstream>
#include <iostream>
#include <sstream>

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

int test(byte* data, int length, float& compr_) {
    // std::ofstream file("../NM", std::ios::binary);
    byte_array compressed;
    byte_array decompressed;
    encode2<1>(data, length, compressed);
    // std::cout << compressed.size() << std::endl;
    // for (byte b : compressed) file << b;
    decode2<1>(compressed.data(), compressed.size(), decompressed);
    // file.close(); 
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
        if (i < 10)
            name << "00" << i;
        else if (i < 100)
            name << "0" << i;
        else
            name << i;
        name << ".jpg";
        int width, height, cnt;
        byte* data = stbi_load(name.str().c_str(), &width, &height, &cnt, 0);
        orig_lengths[i] = width * height * cnt;
        byte_array compressed;
        clock_t start = clock();
        encode0<3>(data, orig_lengths[i], compressed);
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
        decode0<3>(compr_frames[i].data(), compr_lengths[i], decompressed);
        clock_t end = clock();
        decode_time += (double)(end - start) / CLOCKS_PER_SEC;
        if (decompressed.size() != orig_lengths[i]) {
            std::cout << "Error" << std::endl;
            return 1;
        }
        float compr_ = (float)compr_lengths[i] / orig_lengths[i];
        // std::cout << compr_ << std::endl;
        acc += compr_;
    }

    //////////////////
    compr_ = acc / frames;
    return 0;
}

int main() {
    // float compr_ = 0;
    // double encode_time = 0;
    // double decode_time = 0;
    // if (!test_mp4("../imgs/NeuroMatrix.mp4/NeuroMatrix_", 240, compr_,
    // encode_time, decode_time)) std::cout << "________________________\n" <<
    // compr_ << " | " << encode_time
    // << " | " << decode_time <<  std::endl;
    // if (!test_mp4("../imgs/Map.mp4/Map_", 133, compr_, encode_time,
    // decode_time)) std::cout << "________________________\n" << compr_ << " |
    // " << encode_time
    // << " | " << decode_time <<  std::endl;

    // std::string name = "../imgs/Map_default.jpg";
    // int width, height, cnt;
    // byte* data = stbi_load(name.c_str(), &width, &height, &cnt, 0);
    // float compr = 0;
    // test(data, width*height*cnt, compr);
    // std::cout << width*height*cnt << std::endl;

    // std::string name = "../imgs/Map_compressed";
    // byte_array buffer;
    // std::ifstream file(name);
    // byte b;
    // while (file >> b) buffer.push_back(b);
    // file.close();
    // float compr;
    // test(buffer.data(), buffer.size(), compr);
    // std::cout << compr << std::endl;

    // std::ifstream nm1("../imgs/NeuroMatrix_example.bin", std::ios::binary);
    // std::ifstream nm2("../imgs/res3.bin", std::ios::binary);
    // std::ifstream m1("../imgs/Map_example.bin", std::ios::binary);
    // std::ifstream m2("../imgs/res2.bin", std::ios::binary);

    // byte b = 0;
    // while (nm1 >> b) {
    //     byte clone = 0;
    //     nm2 >> clone;
    //     if (b != clone) std::cout << "error" << std::endl; 
    // }  
    // while (m1 >> b) {
    //     byte clone = 0;
    //     m2 >> clone;
    //     if (b != clone) std::cout << "error" << std::endl; 
    // }

    return 0;
}
