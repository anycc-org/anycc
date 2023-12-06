cmake_minimum_required(VERSION 3.24)
project(anycc)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)
include_directories(include/Lex)
include_directories(src)

add_executable(anycc
        include/Lex/DeterministicTransitionDiagramCreator.h
        include/Lex/DFA.h
        include/Lex/FileReader.h
        include/Lex/Input.h
        include/Lex/InputReader.h
        include/Lex/Lex.h
        include/Lex/LexError.h
        include/Lex/NFA.h
        include/Lex/NFAGenerator.h
        include/Lex/NFAState.h
        include/Lex/Operator.h
        include/Lex/Output.h
        include/Lex/OutputReader.h
        include/Lex/Regex.h
        include/Lex/RegexGenerator.h
        include/Lex/Rules.h
        include/Lex/RuleType.h
        include/Lex/SymbolTable.h
        include/Lex/Token.h
        include/Lex/TransitionDiagram.h
        include/Lex/TransitionDiagramMinimizer.h
        include/Lex/TransTable.h
        include/Lex/Utilities.h
        src/Lex/DeterministicTransitionDiagramCreator.cpp
        src/Lex/DFA.cpp
        src/Lex/Input.cpp
        src/Lex/InputReader.cpp
        src/Lex/Lex.cpp
        src/Lex/LexError.cpp
        src/Lex/NFA.cpp
        src/Lex/NFAGenerator.cpp
        src/Lex/NFAState.cpp
        src/Lex/Output.cpp
        src/Lex/OutputReader.cpp
        src/Lex/Regex.cpp
        src/Lex/RegexGenerator.cpp
        src/Lex/Rules.cpp
        src/Lex/SymbolTable.cpp
        src/Lex/Token.cpp
        src/Lex/TransitionDiagram.cpp
        src/Lex/TransitionDiagramMinimizer.cpp
        src/Lex/TransTable.cpp
        src/Lex/Utilities.cpp
        src/anycc.cpp)
