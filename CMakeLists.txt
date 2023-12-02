cmake_minimum_required(VERSION 3.26)
project(anycc)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/Lex)

add_executable(anycc
        include/Lex/DFA.h
        include/Lex/DFAMinimizer.h
        include/Lex/Input.h
        include/Lex/InputReader.h
        include/Lex/Lex.h
        include/Lex/LexError.h
        include/Lex/NFA.h
        include/Lex/NFAGenerator.h
        include/Lex/Output.h
        include/Lex/OutputReader.h
        include/Lex/Regex.h
        include/Lex/RegexGenerator.h
        include/Lex/NFAState.h
        include/Lex/SymbolTable.h
        include/Lex/Token.h
        include/Lex/TransTable.h
        src/Lex/DFA.cpp
        src/Lex/DFAMinimizer.cpp
        src/Lex/Input.cpp
        src/Lex/InputReader.cpp
        src/Lex/Lex.cpp
        src/Lex/LexError.cpp
        src/Lex/NFA.cpp
        src/Lex/NFAGenerator.cpp
        src/Lex/Output.cpp
        src/Lex/OutputReader.cpp
        src/Lex/Regex.cpp
        src/Lex/RegexGenerator.cpp
        src/Lex/SymbolTable.cpp
        src/Lex/Token.cpp
        src/Lex/TransTable.cpp
        src/anycc.cpp
        src/Lex/NFAState.cpp
)
