#pragma once

#include <iostream>
#include "Lex/InputReader.h"
#include "Lex/NFA.h"
#include "Lex/TransitionDiagram.h"
#include "Analyzer.h"


class Lex {
public:
    Lex(std::string *rules_file_name, std::string *program_file_name);

    ~Lex();

    void buildLex();

    void getAllTokensAndCreateOutputFile();

    void printSymbolTable();

private:
    Analyzer *analyzer;
    std::string *program_file_name, *rules_file_name;
    InputReader *inputReader;
    Rules *rules;

    void read_rules();

    /**
     * @brief Build the NFA from the regular expressions
     * @warning the responsibility of deleting pointer is the caller's
     * @return The start state of the NFA
     */
    NFA *buildNFA();

    /**
     * @brief Build the DFA from the NFA
     * @warning the responsibility of deleting pointer is the caller's
     * @param nfa The NFA to be converted to DFA
     * @return TransitionDiagram* The DFA
     */
    TransitionDiagram *buildDFA(NFA *nfa);

    static void printTransitionDiagramStatistics(TransitionDiagram *transition_diagram, const std::string &title);

    static void createDiagramWithDifferentTypes(TransitionDiagram *transition_diagram, const std::string &title);
};