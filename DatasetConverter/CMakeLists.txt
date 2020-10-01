cmake_minimum_required(VERSION 3.10)
project(ConvertDarknetDataset)

set(CMAKE_CXX_STANDARD 14)

add_executable(ConvertDarknetDataset main.cpp progress_bar.cpp)