cmake_minimum_required(VERSION 3.31)
project(ParallelAlgorithms LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(third-party)

include(cmake/CPM.cmake)

add_subdirectory(quicksort)
