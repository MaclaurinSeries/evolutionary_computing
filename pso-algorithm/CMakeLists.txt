cmake_minimum_required(VERSION 3.21)
project(PSO_ALGORITHM)

set(CMAKE_CXX_STANDARD 14)

include_directories("lib")

add_executable(PSO_ALGORITHM
        "lib/particle.cpp"
        "lib/constant.h"
        "lib/pso_algorithm.cpp"
        "lib/pso_algorithm.h"
        "main.cpp"
        "main.h")
