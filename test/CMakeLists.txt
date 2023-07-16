cmake_minimum_required(VERSION 3.0.0)
project(rle_test)

find_package(GTest REQUIRED)

set(test_source main.cpp ../huffman.cpp)
add_executable(tests ${test_source})
target_link_libraries(tests GTest::gtest_main)

target_compile_options(tests PRIVATE -fsanitize=address)
target_link_options(tests PRIVATE -fsanitize=address)
