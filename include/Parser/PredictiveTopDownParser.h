#ifndef ANYCC_PREDICTIVETOPDOWNPARSER_H
#define ANYCC_PREDICTIVETOPDOWNPARSER_H

#include <stack>
#include "PredictiveTable.h"
#include "Token.h"
#include "Lex.h"

/**
 * @brief produce the leftmost derivation for a correct input.
 * If the input grammar is not LL(1), an appropriate error message should be produced.
 */
class PredictiveTopDownParser {
public:
    PredictiveTopDownParser(Lex &lex,
                            const PredictiveTable &predictive_table,
                            const std::set<std::string> &non_terminals,
                            const std::string& filename);

    ~PredictiveTopDownParser();

    /**
     * @brief match the input symbol with top of stack
     * reports an ERROR in case of input mismatch, EMPTY, or SYNC predictive table cell
     * apply the panic mode error recovery in case of ERROR
     * @parm current_token is the input symbol
     */
    void parseInputTokens();

    /**
     * @brief returns the computed productions by the LL(1) parser
     * @returns left most derivation
     * */
    std::vector<std::vector<std::string>> getLeftmostDerivation();

    void printLeftmostDerivation();

    void generateMarkdownLeftmostDerivation(const std::string& filename);

private:
    struct StackItem {
        std::string token;
        bool isTerminal;
    };

    Lex &lex;
    PredictiveTable predictive_table;
    std::set<std::string> non_terminals;
    std::vector<std::vector<std::string>> left_most_derivation;
    std::stack<StackItem> stk;
    std::ofstream parsingFile;

    bool handleMatchOrError(const StackItem& top, Token*& curr_token);
    void handleMatch(const StackItem& top, Token*& curr_token);
    void handleNonTerminal(const StackItem& top, Token*& curr_token);
    void handleMissingTerminalOrSyncEntry(const StackItem& top);
    void handleEmptyEntry(const StackItem& top, Token*& curr_token);
    void handleValidProduction(const StackItem& top, const CellValue* cellValue);
    void handleEndOfInput();
    void pushProductionToStack(const std::vector<std::string>& production);
    void setNextDerivation(const StackItem& top, std::vector<std::string> &curr_production);
};

#endif //ANYCC_PREDICTIVETOPDOWNPARSER_H
