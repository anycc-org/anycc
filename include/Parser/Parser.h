#ifndef ANYCC_PARSER_H
#define ANYCC_PARSER_H


#include <string>
#include "FirstAndFollowGenerator.h"
#include "PredictiveTable.h"
#include "PredictiveTopDownParser.h"

class Parser {
public:
    Parser(std::string &cfg_file_name, Lex *lex);

    ~Parser();

    void buildParser();

    void parseProgram();

private:
    std::string cfg_file_name;
    Lex *lex;
    FirstAndFollowGenerator *firstAndFollowGenerator;
    PredictiveTable *predictiveTable;
    PredictiveTopDownParser *predictiveTopDownParser;

    static std::unordered_map<std::string, std::vector<std::vector<std::string>>> buildCFG();

    void buildFirstAndFollowSets(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar);

    void buildPredictiveTable();

    void buildPredictiveTopDownParser();
};


#endif //ANYCC_PARSER_H
