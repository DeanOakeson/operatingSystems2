cmake_minimum_required(VERSION 3.15)
project(vm VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic)

add_library(hardware STATIC
    vm.cpp
    vm.h
    ram.h
    pcb.h
)

target_include_directories(hardware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(hardware PUBLIC cxx_std_20)
