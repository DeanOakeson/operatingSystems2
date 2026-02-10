cmake_minimum_required(VERSION 3.15)
project(dos VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic)

add_subdirectory(src/hardware)
add_subdirectory(src/kernel)
add_subdirectory(src/shell)

add_executable(dos src/main.cpp)

target_link_libraries(dos PRIVATE
    shell
    kernel
    hardware
)
