cmake_minimum_required(VERSION 3.14)
project(rle VERSION 0.1.0)

set(source main.cpp rle.cpp)
include_directories(include)

add_executable(main ${source})
add_subdirectory(test)
