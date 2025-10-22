cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0110)
  cmake_policy(SET CMP0110 NEW)
endif()

if(POLICY CMP0175)
  cmake_policy(SET CMP0175 NEW)
endif()

project(c-datastructures C)

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(ClangFormat)
enable_clang_format()

find_package(FlawFinder)
enable_flaw_finder()

add_subdirectory(src)
add_subdirectory(test)
