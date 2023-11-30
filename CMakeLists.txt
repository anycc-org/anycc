cmake_minimum_required(VERSION 3.10)
project(asyncc)

set(CMAKE_CXX_STANDARD 14)

add_executable(main main.cpp GrammarChecker.cpp)
