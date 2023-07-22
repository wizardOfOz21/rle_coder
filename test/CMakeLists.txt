cmake_minimum_required(VERSION 3.14)
project(rle_test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GTEST_ROOT "/Program Files (x86)/googletest-distribution")
find_package(GTest REQUIRED)

add_executable(c_rle_tests c_rle.cpp)
target_link_libraries(c_rle_tests PRIVATE GTest::GTest)

add_executable(basic_rle_tests basic.cpp)
target_link_libraries(basic_rle_tests PRIVATE GTest::GTest)

add_executable(ver0_tests version0.cpp)
target_link_libraries(ver0_tests PRIVATE GTest::GTest)
