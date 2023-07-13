cmake_minimum_required(VERSION 3.0.0)
project(rle VERSION 0.1.0)

include(CTest)
enable_testing()

set(source main.cpp rle.cpp)
include_directories(include)

add_executable(main ${source})

target_compile_options(main PRIVATE -fsanitize=address)
target_link_options(main PRIVATE -fsanitize=address)

add_subdirectory(test)
