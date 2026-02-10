cmake_minimum_required(VERSION 3.15)
project(kernel VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic)

add_library(kernel STATIC
    kernel.cpp
    loader.cpp
    errorhandler.cpp
    scheduler.cpp
)

target_include_directories(kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Kernel depends on hardware
target_link_libraries(kernel PUBLIC hardware)

target_compile_features(kernel PUBLIC cxx_std_20)
