cmake_minimum_required(VERSION 3.26)
project(anycc)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/Lex)

add_executable(anycc
        src/Lex/Input.cpp
        src/Lex/InputReader.cpp
        src/Lex/Lex.cpp
        src/Lex/Rules.cpp
        src/Lex/DFA.cpp
        src/Lex/DFAMinimizer.cpp
        src/Lex/LexError.cpp
        src/Lex/NFA.cpp
        src/Lex/NFAGenerator.cpp
        src/Lex/NFAState.cpp
        src/Lex/Output.cpp
        src/Lex/OutputReader.cpp
        src/Lex/Regex.cpp
        src/Lex/RegexGenerator.cpp
        src/Lex/SymbolTable.cpp
        src/Lex/Token.cpp
        src/Lex/TransTable.cpp
        src/anycc.cpp
        include/Lex/Utilities.h
        src/Lex/Utilities.cpp
        include/Lex/Operator.h
)
