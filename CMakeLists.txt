cmake_minimum_required(VERSION 3.24)
project(anycc)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(anycc
        constants.h
        GrammarChecker.cpp
        GrammarChecker.h
        GrammarCheckerUtility.cpp
        GrammarCheckerUtility.h
        main.cpp
        Production.h)
