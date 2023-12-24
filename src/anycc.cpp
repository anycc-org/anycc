#include "Lex/Lex.h"
#include "constants.h"
#include <set>
#include "Parser/Parser.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <rules_file_name> <cfg_file_name> <program_file_name>\n";
        return 1;
    }

    std::string rules_file_name = argv[1];
    std::string cfg_file_name = argv[2];
    std::string program_file_name = argv[3];

    auto lex = new Lex(rules_file_name, program_file_name);
    lex->buildLex();

    auto parser = new Parser(cfg_file_name, lex);
    parser->buildParser();
    parser->parseProgram();

    SymbolTable::getInstance().printTable();

    return 0;
}
