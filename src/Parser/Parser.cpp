#include "Parser/Parser.h"
#include "Utilities.h"

Parser::Parser(std::string &cfg_file_name, Lex *lex) {
    this->cfg_file_name = cfg_file_name;
    this->lex = lex;
}

Parser::~Parser() {
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
    predictiveTopDownParser->generateMarkdownLeftmostDerivation("../LeftmostDerivation.md");
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
    predictiveTable->buildPredictiveTable();
    std::cout << "\nPredictive Table:\n";
    predictiveTable->printPredictiveTable();
    predictiveTable->generateMarkdownTable("../PredictiveTable.md");
}

void Parser::buildPredictiveTopDownParser() {
    std::cout << "\nLL(1) parser\n";
    predictiveTopDownParser = new PredictiveTopDownParser(*lex, *predictiveTable,
                                                          firstAndFollowGenerator->getNonTerminals(),
                                                          "../LL1ParsingOutput.md");
}
