cmake_minimum_required(VERSION 3.14)
project(rle VERSION 0.1.0)

include_directories(include)

add_executable(main main.cpp rle.cpp)
add_executable(compress compress.cpp)
add_executable(decompress decompress.cpp)
add_subdirectory(test)
