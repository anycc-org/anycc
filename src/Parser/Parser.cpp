#include "Parser/Parser.h"
#include "Utilities.h"

Parser::Parser(std::string *cfg_file_name, Lex *lex) {
    this->cfg_file_name = cfg_file_name;
    this->lex = lex;
}

Parser::~Parser() {
    delete cfg_file_name;
    delete firstAndFollowGenerator;
    delete predictiveTable;
    delete predictiveTopDownParser;
}

void Parser::buildParser() {
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = buildCFG();
    buildFirstAndFollowSets(grammar);
    buildPredictiveTable();
    buildPredictiveTopDownParser();
}

void Parser::parseProgram() {
    predictiveTopDownParser->parseInputTokens();
    predictiveTopDownParser->printLeftmostDerivation();
}

std::unordered_map<std::string, std::vector<std::vector<std::string>>> Parser::buildCFG() {
    return Utilities::parseCFGInput("../CFG.txt");
}

void Parser::buildFirstAndFollowSets(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar) {
    firstAndFollowGenerator = new FirstAndFollowGenerator(grammar);
    firstAndFollowGenerator->compute();
}

void Parser::buildPredictiveTable() {
    predictiveTable = new PredictiveTable(firstAndFollowGenerator->getFirstSets(),
                                          firstAndFollowGenerator->getFollowSets(),
                                          firstAndFollowGenerator->getNonTerminals());
    std::cout << "\nPredictive Table:\n";
    predictiveTable->buildPredictiveTable();
}

void Parser::buildPredictiveTopDownParser() {
    std::cout << "\nLL(1) parsing:\n";
    predictiveTopDownParser = new PredictiveTopDownParser(*lex, *predictiveTable,
                                                          firstAndFollowGenerator->getNonTerminals());
}
