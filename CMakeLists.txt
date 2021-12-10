cmake_minimum_required(VERSION 3.20)
project (WavGen)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(sinewav)

add_executable(WavGen main.cpp)
target_link_libraries(WavGen SineWave)