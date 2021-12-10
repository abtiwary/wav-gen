cmake_minimum_required(VERSION 3.20)
project (SineWave)

set(CMAKE_CXX_STANDARD 17)

add_library(SineWave src/sinewav.cpp include/sinewav.hpp include/wav.hpp)
include_directories(include)
