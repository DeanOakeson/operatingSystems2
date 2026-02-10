cmake_minimum_required(VERSION 3.15)
project(shell VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic)

add_library(shell STATIC
    shell.cpp
)

target_include_directories(shell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Shell depends on kernel (which already includes hardware)
target_link_libraries(shell PUBLIC kernel)

target_compile_features(shell PUBLIC cxx_std_20)
