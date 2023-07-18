#include <cstdlib>
#include <sstream>
#include <vector>

#include "gtest/gtest.h"
#include "rle.hpp"

void encode(const byte* original, int length, byte_array& compressed) {
    encode2(original, length, compressed);
};

void decode(const byte* compressed, int length, byte_array& original) {
    decode2(compressed, length, original);
};

std::string to_str(const byte_array &res) {
    std::stringstream out;
    for (char b : res) {
        if ('a' <= b && b <= 'z')
            out << (char)b;
        else
            out << (int)b;
    }
    return std::move(out.str());
}

// TEST(RLE_Decode_Test, RLE_One_Symbol_Test) {
//     std::string str = "a";
//     byte_array res;
//     encode((byte *)str.c_str(), str.length(), res);
//     byte_array orig;
//     decode(res.data(), res.size(), orig);
//     std::string compr = to_str(orig);
//     EXPECT_EQ(compr, str);
// }

TEST(RLE_Decode_Test, RLE_Just_Seq_Test) {
    std::string str = "aaaaaa";
    byte_array res;
    encode((byte *)str.c_str(), str.length(), res);
    byte_array orig;
    decode(res.data(), res.size(), orig);
    std::string compr = to_str(orig);
    EXPECT_EQ(compr, str);
}

TEST(RLE_Decode_Test, RLE_Two_Seqs_Test) {
    std::string str = "aaaabb";
    byte_array res;
    encode((byte *)str.c_str(), str.length(), res);
    byte_array orig;
    decode(res.data(), res.size(), orig);
    std::string compr = to_str(orig);
    EXPECT_EQ(compr, str);
}

TEST(RLE_Decode_Test, RLE_Just_NSeq_Test) {
    std::string str = "ababab";
    byte_array res;
    encode((byte *)str.c_str(), str.length(), res);
    byte_array orig;
    decode(res.data(), res.size(), orig);
    std::string compr = to_str(orig);
    EXPECT_EQ(compr, str);
}

TEST(RLE_Decode_Test, RLE_Seq_NSeq_Test) {
    std::string str = "aaabac";
    byte_array res;
    encode((byte *)str.c_str(), str.length(), res);
    byte_array orig;
    decode(res.data(), res.size(), orig);
    std::string compr = to_str(orig);
    EXPECT_EQ(compr, str);
}

TEST(RLE_Decode_Test, RLE_NSeq_Seq_Test) {
    std::string str = "ababaaaaaaaa";
    byte_array res;
    encode((byte *)str.c_str(), str.length(), res);
    byte_array orig;
    decode(res.data(), res.size(), orig);
    std::string compr = to_str(orig);
    EXPECT_EQ(compr, str);
}

TEST(RLE_Decode_Test, RLE_Seq_NSeq_Seq_Test) {
    std::string str = "aaabacaaaaaa";
    byte_array res;
    encode((byte *)str.c_str(), str.length(), res);
    byte_array orig;
    decode(res.data(), res.size(), orig);
    std::string compr = to_str(orig);
    EXPECT_EQ(compr, str);
}

TEST(RLE_Decode_Test, RLE_One_InTheEnd_Test) {
    std::string str = "aaaaab";
    byte_array res;
    encode((byte *)str.c_str(), str.length(), res);
    byte_array orig;
    decode(res.data(), res.size(), orig);
    std::string compr = to_str(orig);
    EXPECT_EQ(compr, str);
}

TEST(RLE_Decode_Test, RLE_MoreThan127Seq_Test) {
    std::string str(258, 'a');
    byte_array res;
    encode((byte *)str.c_str(), str.length(), res);
    byte_array orig;
    decode(res.data(), res.size(), orig);
    std::string compr = to_str(orig);
    EXPECT_EQ(compr, str);
}

TEST(RLE_Decode_Test, RLE_MoreThan127NSeq_Test) {
    std::string str;
    for (int i = 0; i < 129; ++i) {
        str += "ab";
    }
    byte_array res;
    encode((byte *)str.c_str(), str.length(), res);
    byte_array orig;
    decode(res.data(), res.size(), orig);
    std::string compr = to_str(orig);
    EXPECT_EQ(compr, str);
}

TEST(RLE_Random_Test, RLE_Random1_Test) {
    const int REP = 1000;
    for (int i = 0; i < REP; ++i) {
        byte_array orig;
        int count = 6 * (std::rand() % 250 + 1);
        for (int j = 0; j < count; ++j) {
            orig.push_back(std::rand() % 256);
        }
        byte_array comp;
        encode(orig.data(), count, comp);
        byte_array decomp;
        decode(comp.data(), comp.size(), decomp);
        // for (int j = 0; j < count; ++j) {
        //     EXPECT_EQ(orig[j], decomp[j]);
        //     if (orig[j] != decomp[j]) {
        //         int a  =1;
        //     }
        // }
        EXPECT_EQ(orig, decomp);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
