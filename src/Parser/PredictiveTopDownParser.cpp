#include "Parser/PredictiveTopDownParser.h"
#include "constants.h"

PredictiveTopDownParser::PredictiveTopDownParser(
        Lex &lex,
        const PredictiveTable &predictive_table,
        const std::set<std::string> &non_terminals)
        : lex(lex), predictive_table(predictive_table), non_terminals(non_terminals) {
    stk.push({"$", true});
    stk.push({START_SYMBOL, false});
    left_most_derivation.push_back({START_SYMBOL});
}

PredictiveTopDownParser::~PredictiveTopDownParser() {}

void PredictiveTopDownParser::parseInputTokens() {
    // get token for the first time
    Token *curr_token = lex.getNextToken();
    while (!stk.empty()) {
        auto top = stk.top();
        std::cout << "stack top: " << top.token << " ,curr token: " << *(curr_token->getKey());

        if (top.isTerminal) {
            if (top.token == *(curr_token->getValue())) {
                // match
                std::cout << " ,matched " << curr_token << std::endl;
                stk.pop();
                // get next token
                curr_token = lex.getNextToken();
                continue; // for convenience
            } else {
                // panic mode error recovery
                std::cerr << "Error: missing " << top.token << ", inserted" << std::endl;
                stk.pop();
                continue; // for convenience
            }
        }
        else{
            // stack top is non-terminal
            const CellValue *cellValue = predictive_table.lookUp(top.token, *(curr_token->getKey()));
            ParsingTableEntryType entryType = cellValue->getPredictiveTableEntryType();

            if (entryType == ParsingTableEntryType::EMPTY) {
                // panic mode error recovery
                std::cerr << "Error:(illegal " << top.token << ") – discard " << curr_token << std::endl;
                // get next token
                curr_token = lex.getNextToken();
                continue;
            }
            if (entryType == ParsingTableEntryType::SYNC) {
                // panic mode error recovery
                std::cerr << "Error: missing " << top.token << ", discarded" << std::endl;
                stk.pop();
                continue;
            }
            if (entryType == ParsingTableEntryType::VALID_PRODUCTION) {
                stk.pop();
                auto production = cellValue->getProduction().productions[0];

                // construct next derivation
                setNextDerivation(top.token, production);

                // Loop through the vector in reverse order
                for (auto it = production.rbegin(); it != production.rend(); ++it) {
                    // Push instances into the stack
                    bool isTerminal = non_terminals.find(*it) == non_terminals.end();
                    stk.push({*it, isTerminal});
                }
                if (stk.top().token == EPSILON) {
                    stk.pop();
                }
                continue;
            }
        }
    }
}

/**
 * @brief construct next derivation from last derivation and curr_production
 * */
void PredictiveTopDownParser::setNextDerivation(
        std::string &non_terminal, std::vector<std::string> &curr_production) {

    std::vector<std::string> next_derivation;
    auto last_derivation = left_most_derivation.back(); // id T' E'

    auto it = last_derivation.begin();
    for (; it != last_derivation.end(); ++it) {
        if (*it == non_terminal) {
            ++it;
            break;
        }
        next_derivation.push_back(*it);
    }

    for (auto& symbol: curr_production) {
        if (symbol == EPSILON) {
            continue;
        }
        next_derivation.push_back(symbol);
    }

    for (; it != last_derivation.end(); ++it) {
        next_derivation.push_back(*it);
    }

    left_most_derivation.push_back(next_derivation);
}

std::vector<std::vector<std::string>> PredictiveTopDownParser::getLeftmostDerivation() {
    return left_most_derivation;
}

void PredictiveTopDownParser::printLeftmostDerivation() {
    std::cout << "\nLeftmost derivation:\n";
    for (const auto& production: left_most_derivation) {
        for (const auto& sentential: production) {
            std::cout << sentential << " ";
        }
        std::cout << std::endl;
    }
}