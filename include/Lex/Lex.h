#pragma once

#include <iostream>
#include "Lex/InputReader.h"
#include "Lex/NFA.h"
#include "Lex/TransitionDiagram.h"
#include "Lex/Analyzer.h"


class Lex {
public:
    Lex(std::string &rules_file_name, std::string &program_file_name);

    ~Lex();

    /**
     * @brief Build the NFA and DFA and analyze the program to create the output file with the tokens
     */
    void buildLex();

    /**
     * @brief Get all tokens and print them and create the output file
     */
    void getAllTokensAndCreateOutputFile();

    Token *getNextToken();

    /**
     * @brief Print the symbol table
     */
    void printSymbolTable();

private:
    Analyzer *analyzer;
    std::string program_file_name, rules_file_name;
    InputReader *inputReader;
    Rules *rules;

    /**
     * @brief Read the rules file and build the rules object
     */
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

    /**
     * @brief Print the statistics of the given transition diagram either (NFA, DFA, minimized DFA)
     * @param transition_diagram The transition diagram to be printed
     * @param title The title of the transition diagram
     */
    static void printTransitionDiagramStatistics(TransitionDiagram *transition_diagram, const std::string &title);

    /**
     * @brief Create the transition diagram with different types (NFA, DFA, minimized DFA)
     * @param transition_diagram The transition diagram to be printed
     * @param title The title of the transition diagram
     */
    static void createDiagramWithDifferentTypes(TransitionDiagram *transition_diagram, const std::string &title);
};