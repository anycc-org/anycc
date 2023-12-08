#pragma once

#include <iostream>
#include "Lex/InputReader.h"
#include "Lex/NFA.h"


class Lex {
public:
    Lex(std::string *rules_file_name, std::string *program_file_name);

    ~Lex();

    void buildLex();

private:
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
};