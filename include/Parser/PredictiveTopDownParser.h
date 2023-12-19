#ifndef ANYCC_PREDICTIVETOPDOWNPARSER_H
#define ANYCC_PREDICTIVETOPDOWNPARSER_H

#include <stack>
#include "PredictiveTable.h"
#include "Token.h"

/**
 * @brief produce the leftmost derivation for a correct input.
 * If the input grammar is not LL(1), an appropriate error message should be produced.
 */
class PredictiveTopDownParser {
public:
    PredictiveTopDownParser(const PredictiveTable &predictive_table, const std::set<std::string> &non_terminals);

    ~PredictiveTopDownParser();

    /**
     * @brief match the input symbol with top of stack
     * reports an ERROR in case of input mismatch, EMPTY, or SYNC predictive table cell
     * apply the panic mode error recovery in case of ERROR
     * @parm current_token is the input symbol
     */
    void processNextToken(const std::string &current_token);

    /**
     * @brief returns the computed productions by the LL(1) parser
     * @returns left most derivation
     * */
    std::vector<std::vector<std::string>> getLeftmostDerivation();

    void printLeftmostDerivation();

private:
    struct StackEntry {
        std::string token;
        bool isTerminal;
    };

    PredictiveTable predictive_table;
    std::set<std::string> non_terminals;
    std::vector<std::vector<std::string>> left_most_derivation;
    std::stack<StackEntry> stk;
//    std::stack<std::string> left_derivation_stack;

    void setNextDerivation(std::string &non_terminal, std::vector<std::string> &curr_production);
};

#endif //ANYCC_PREDICTIVETOPDOWNPARSER_H
