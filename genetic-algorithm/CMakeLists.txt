cmake_minimum_required(VERSION 3.21)
project(GENETIC_ALGORITHM)

set(CMAKE_CXX_STANDARD 14)

include_directories("lib")

add_executable(GENETIC_ALGORITHM
        "lib/chromosome.cpp"
        "lib/constant.h"
        "lib/genetic_algorithm.cpp"
        "lib/genetic_algorithm.h"
        "main.cpp"
        "main.h")
