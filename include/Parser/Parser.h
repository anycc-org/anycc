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

    /**
     * @brief Build the cfg, first and follow sets, predictive table, and predictive top down parser
     */
    void buildParser();

    /**
     * @brief Parse the program using the predictive top down parser
     */
    void parseProgram();

private:
    std::string cfg_file_name;
    Lex *lex;
    FirstAndFollowGenerator *firstAndFollowGenerator;
    PredictiveTable *predictiveTable;
    PredictiveTopDownParser *predictiveTopDownParser;

    /**
     * @brief Build the cfg
     * @return The cfg
     */
    static std::unordered_map<std::string, std::vector<std::vector<std::string>>> buildCFG();

    /**
     * @brief Build the first and follow sets
     * @param grammar The cfg
     */
    void buildFirstAndFollowSets(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar);

    /**
     * @brief Build the predictive table
     */
    void buildPredictiveTable();

    /**
     * @brief Build the predictive top down parser
     */
    void buildPredictiveTopDownParser();
};


#endif //ANYCC_PARSER_H
