#include "Parser/Parser.h"
#include "Parser/CFGReader.h"
#include "LeftRecursionRemover.h"
#include "LeftFactorer.h"

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
    auto grammar = CFGReader::parseCFGInput("../CFG.txt");
    auto lr_free_grammar = LeftRecursionRemover::removeLR(grammar);
    auto left_factored_grammar = LeftFactorer::leftFactor(lr_free_grammar);
    printGrammar(left_factored_grammar);
    return left_factored_grammar;
}

void Parser::buildFirstAndFollowSets(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar) {
    firstAndFollowGenerator = new FirstAndFollowGenerator(grammar);
    firstAndFollowGenerator->compute();
    firstAndFollowGenerator->generateMarkdownFirstAndFollowSets("../FirstAndFollowSets.md");
}

void Parser::buildPredictiveTable() {
    predictiveTable = new PredictiveTable(firstAndFollowGenerator->getFirstSets(),
                                          firstAndFollowGenerator->getFollowSets(),
                                          firstAndFollowGenerator->getNonTerminals());
    predictiveTable->buildPredictiveTable();
    predictiveTable->generateMarkdownTable("../PredictiveTable.md");
}

void Parser::buildPredictiveTopDownParser() {
    std::cout << "\nLL(1) parser\n";
    predictiveTopDownParser = new PredictiveTopDownParser(*lex, *predictiveTable,
                                                          firstAndFollowGenerator->getNonTerminals(),
                                                          "../LL1ParsingOutput.md");
}

void Parser::printGrammar(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar) {
    std::cout << "Grammar:\n";
    for (auto &non_terminal: grammar) {
        std::cout << non_terminal.first << " --> ";
        int i = 0;
        for (auto &production: non_terminal.second) {
            i++;
            for (auto &symbol: production) {
                std::cout << symbol << " ";
            }
            if (i < non_terminal.second.size())
                std::cout << "| ";
        }
        std::cout << "\n";
    }
}
