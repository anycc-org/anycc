#include "Lex/Lex.h"
#include "Parser/FirstAndFollowGenerator.h"
#include "constants.h"
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include "Utilities.h"
#include "Parser/PredictiveTable.h"
#include "Parser/PredictiveTopDownParser.h"

int main() {
    auto *rules_file_name = new std::string("../rules.txt");
    auto *program_file_name = new std::string("../program.txt");

    Lex *lex = new Lex(rules_file_name, program_file_name);
    lex->buildLex();

    lex->getAllTokensAndCreateOutputFile();
    lex->printSymbolTable();

    return 0;
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = Utilities::parseCFGInput(
            "../CFG.txt");
    FirstAndFollowGenerator firstAndFollowGenerator(grammar);
    firstAndFollowGenerator.compute();
    // Print first_sets
    const auto &first_sets = firstAndFollowGenerator.getFirstSets();
    std::cout << "First Sets:\n";
    for (const auto &entry: first_sets) {
        const std::string &non_terminal = entry.first;
        const std::set<std::pair<std::string, Production>, CompareFirst> &first_set = entry.second;

        std::cout << non_terminal << ": { ";
        for (std::pair<std::string, Production> symbol: first_set) {
            std::cout << symbol.first << ' ';
        }
        std::cout << "}\n";
    }

    // Print followSets
    const auto &followSets = firstAndFollowGenerator.getFollowSets();
    std::cout << "\nFollow Sets:\n";
    for (const auto &entry: followSets) {
        const std::string &non_terminal = entry.first;
        const std::set<std::string> &follow_set = entry.second;

        std::cout << non_terminal << ": { ";
        for (std::string symbol: follow_set) {
            std::cout << symbol << ' ';
        }
        std::cout << "}\n";
    }

    // build predictive table
    PredictiveTable predictiveTable(first_sets, followSets, firstAndFollowGenerator.getNonTerminals());
    predictiveTable.buildPredictiveTable();
    std::cout << "\nPredictive Table:\n";
    predictiveTable.printPredictiveTable();

    // LL(1) parsing
    std::cout << "\nLL(1) parsing:\n";
    PredictiveTopDownParser parser(predictiveTable, firstAndFollowGenerator.getNonTerminals());
    // parsing input: id+id$
    parser.processNextToken("id");
    parser.processNextToken("+");
    parser.processNextToken("id");
    parser.processNextToken("$");
    parser.printLeftmostDerivation();

    return 0;
}
