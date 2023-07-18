#include <cstdlib>
#include <sstream>
#include <vector>

#include "gtest/gtest.h"
#include "rle.hpp"

void encode(const byte* original, int length, byte_array& compressed) {
    encode0(original, length, compressed);
};

void decode(const byte* compressed, int length, byte_array& original) {
    decode0(compressed, length, original);
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

TEST(RLE_Encode_Test, RLE_One_Symbol_Test) {
    std::string str = "a";
    byte_array res;

    encode((byte *)str.c_str(), str.length(), res);
    std::string compr = to_str(res);
    EXPECT_EQ(compr, "-1a");
}

TEST(RLE_Encode_Test, RLE_Just_Seq_Test) {
    std::string str = "aaaa";
    byte_array res;

    encode((byte *)str.c_str(), str.length(), res);
    std::string compr = to_str(res);
    EXPECT_EQ(compr, "4a");
}

TEST(RLE_Encode_Test, RLE_Two_Seqs_Test) {
    std::string str = "aaaabb";
    byte_array res;

    encode((byte *)str.c_str(), str.length(), res);
    std::string compr = to_str(res);
    EXPECT_EQ(compr, "4a2b");
}

TEST(RLE_Encode_Test, RLE_Just_NSeq_Test) {
    std::string str = "ababa";
    byte_array res;

    encode((byte *)str.c_str(), str.length(), res);
    std::string compr = to_str(res);
    EXPECT_EQ(compr, "-5ababa");
}

TEST(RLE_Encode_Test, RLE_Seq_NSeq_Test) {
    std::string str = "aaaba";
    byte_array res;

    encode((byte *)str.c_str(), str.length(), res);
    std::string compr = to_str(res);
    EXPECT_EQ(compr, "3a-2ba");
}

TEST(RLE_Encode_Test, RLE_NSeq_Seq_Test) {
    std::string str = "ababaaa";
    byte_array res;

    encode((byte *)str.c_str(), str.length(), res);
    std::string compr = to_str(res);
    EXPECT_EQ(compr, "-4abab3a");
}

TEST(RLE_Encode_Test, RLE_Seq_NSeq_Seq_Test) {
    std::string str = "aaabacaaa";
    byte_array res;

    encode((byte *)str.c_str(), str.length(), res);
    std::string compr = to_str(res);
    EXPECT_EQ(compr, "3a-3bac3a");
}

TEST(RLE_Encode_Test, RLE_One_InTheEnd_Test) {
    std::string str = "aaab";
    byte_array res;

    encode((byte *)str.c_str(), str.length(), res);
    std::string compr = to_str(res);
    EXPECT_EQ(compr, "3a-1b");
}

TEST(RLE_Encode_Test, RLE_MoreThan127Seq_Test) {
    std::string str(255, 'a');
    byte_array res;

    encode((byte *)str.c_str(), str.length(), res);
    std::string compr = to_str(res);
    EXPECT_EQ(compr, "127a127a1a");
}

TEST(RLE_Encode_Test, RLE_MoreThan127NSeq_Test) {
    std::string str;
    for (int i = 0; i < 127; ++i) {
        str += "ab";
    }
    byte_array res;

    encode((byte *)str.c_str(), str.length(), res);
    std::string compr = to_str(res);
    std::stringstream correct;
    correct << -127;
    for (int i = 0; i < 63; i++) correct << "ab";
    correct << 'a' << -127 << 'b';
    for (int i = 0; i < 63; i++) correct << "ab";
    EXPECT_EQ(compr, correct.str());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
