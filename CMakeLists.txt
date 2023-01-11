cmake_minimum_required(VERSION 3.20)
project(options_pricing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES
    src/*.hpp
    src/*.cpp)
list(REMOVE_ITEM SOURCES "src/main.cpp")

add_executable(options_pricing ${SOURCES} "src/main.cpp")
target_include_directories(options_pricing PUBLIC "include")