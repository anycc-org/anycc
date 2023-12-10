cmake_minimum_required(VERSION 3.24)
project(anycc)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/Lex)

add_executable(anycc
        constants.h
        GrammarChecker.cpp
        GrammarChecker.h
        GrammarCheckerUtility.cpp
        GrammarCheckerUtility.h
        main.cpp
        Production.h
        src/Lex/InputReader.cpp
        src/Lex/Lex.cpp
        src/Lex/Rules.cpp
        src/Lex/NFA.cpp
        src/Lex/NFAGenerator.cpp
        src/Lex/NFAState.cpp
        src/Lex/SymbolTable.cpp
        src/Lex/Token.cpp
        src/anycc.cpp
        src/Lex/Utilities.cpp
        include/Lex/Operator.h
        src/Lex/TransitionDiagram.cpp
        src/Lex/Analyzer.cpp
        include/Lex/FileReader.h
        src/Lex/DeterministicTransitionDiagramCreator.cpp
        src/Lex/TransitionDiagramMinimizer.cpp
        include/Lex/RuleType.h
        include/Lex/Token.h
)