#pragma once

#include <iostream>
#include "InputReader.h"


class Lex {
public:
    Lex(std::string *rules_file_name, std::string *program_file_name);

    ~Lex();

    void read_rules();

private:
    std::string *program_file_name, *rules_file_name;
    InputReader *inputReader;
    Rules *rules;
};