cmake_minimum_required(VERSION 3.24)
project(anycc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
        HEADER_FILES

        include/Lex/Analyzer.h
        include/Lex/DeterministicTransitionDiagramCreator.h
        include/Lex/Epsilon.h
        include/Lex/FileReader.h
        include/Lex/InputReader.h
        include/Lex/Lex.h
        include/Lex/NFA.h
        include/Lex/NFAGenerator.h
        include/Lex/NFAState.h
        include/Lex/Operator.h
        include/Lex/Rules.h
        include/Lex/RuleType.h
        include/Lex/Token.h
        include/Lex/TransitionDiagram.h
        include/Lex/TransitionDiagramMinimizer.h
        include/Lex/Utilities.h

        include/Parser/CellKey.h
        include/Parser/CellValue.h
        include/Parser/CFGReader.h
        include/Parser/FirstAndFollowGenerator.h
        include/Parser/FirstAndFollowGeneratorUtility.h
        include/Parser/LeftFactorer.h
        include/Parser/LeftRecursionRemover.h
        include/Parser/Parser.h
        include/Parser/ParsingTableEntryType.h
        include/Parser/PredictiveTable.h
        include/Parser/PredictiveTopDownParser.h
        include/Parser/Production.h

        include/constants.h
        include/SymbolTable.h
        include/SubstringInfo.h
)

set(
        SOURCE_FILES

        src/Lex/Analyzer.cpp
        src/Lex/DeterministicTransitionDiagramCreator.cpp
        src/Lex/InputReader.cpp
        src/Lex/Lex.cpp
        src/Lex/NFA.cpp
        src/Lex/NFAGenerator.cpp
        src/Lex/NFAState.cpp
        src/Lex/Rules.cpp
        src/Lex/Token.cpp
        src/Lex/TransitionDiagram.cpp
        src/Lex/TransitionDiagramMinimizer.cpp
        src/Lex/Utilities.cpp

        src/Parser/CellKey.cpp
        src/Parser/CellValue.cpp
        src/Parser/CFGReader.cpp
        src/Parser/FirstAndFollowGenerator.cpp
        src/Parser/FirstAndFollowGeneratorUtility.cpp
        src/Parser/LeftFactorer.cpp
        src/Parser/LeftRecursionRemover.cpp
        src/Parser/Parser.cpp
        src/Parser/PredictiveTable.cpp
        src/Parser/PredictiveTopDownParser.cpp

        src/SymbolTable.cpp
)

add_library(anycc_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})

include_directories(include)
include_directories(include/Lex)
include_directories(include/Parser)

add_executable(anycc
        ${SOURCE_FILES}
        src/anycc.cpp
)

add_subdirectory(tests)