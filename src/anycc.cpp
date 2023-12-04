#include <iostream>
#include "Lex.h"


int main() {
    auto *rules_file_name = new std::string("../rules.txt");
    auto *program_file_name = new std::string("../program.txt");
    Lex *lex = new Lex(rules_file_name, program_file_name);
    lex->read_rules();
}