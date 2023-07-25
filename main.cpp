#include <time.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "rle.hpp"
#include "practice/modelA.h"
#include "practice/compressor.h"
#include "practice/decompressor.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

int test(byte_* data, int length, float& compr_) {
    byte_array compressed;
    byte_array decompressed;
    encode2<1>(data, length, compressed);
    decode2<1>(compressed.data(), compressed.size(), decompressed);
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
        byte_* data = stbi_load(name.str().c_str(), &width, &height, &cnt, 0);
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
   // RLE
    // Reading 
    std::string input_name = "../imgs/Map_default.jpg";
    std::string compressed_name = "../imgs/compressed.bin";
    std::string decompressed_name = "../imgs/Map_origin";
    int width, height, cnt;
    byte_* data = stbi_load(input_name.c_str(), &width, &height, &cnt, 0);

   // Сompression
    byte_array compressed;
    byte_array decompressed;
    int length = width*height*cnt;
    encode0<3>(data, length, compressed);   
    decode0<3>(compressed.data(), compressed.size(), decompressed);
    float compr_ = (float)compressed.size() / length;
    std::cout << "RLE-compressed data size: " << 100*compr_ << "%" << std::endl;

   // Writing
    std::string rle_name = decompressed_name + "_rle.jpg";
    stbi_write_jpg(rle_name.data(), width, height, cnt, decompressed.data(), 100);
   ///


   // Arithmetic coding
    ifstream input(input_name, std::ios::binary);
    ofstream output(compressed_name, std::ios::binary);
    modelA<int, 16, 14> cmodel;
    compress(input, output, cmodel);
    input.close();
    output.close();

    ifstream compressed_(compressed_name, std::ios::binary);
    std::string arith_name = decompressed_name + "_arith.jpg";
    ofstream decompressed_(arith_name, std::ios::binary);
    modelA<int, 16, 14> cmodel_;
    decompress(compressed_, decompressed_, cmodel_);
    compressed_.close();
    decompressed_.close();
   ///

    return 0;
}
