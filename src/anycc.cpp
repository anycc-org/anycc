#include "Lex/Lex.h"
#include "constants.h"
#include <set>
#include "Parser/Parser.h"

int main() {
    auto rules_file_name = new std::string("../rules.txt");
    auto program_file_name = new std::string("../program.txt");

    auto lex = new Lex(rules_file_name, program_file_name);
    lex->buildLex();

    auto cfg_file_name = new std::string("../CFG.txt");
    auto parser = new Parser(cfg_file_name, lex);
    parser->buildParser();
    parser->parseProgram();

    return 0;
}
