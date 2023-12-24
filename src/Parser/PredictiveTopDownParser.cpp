#include <iomanip>
#include <iostream>
#include <fstream>
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
    std::cout << "Parsing input tokens..." << std::endl;
    std::cout << "+--------+------------+" << std::endl;
    std::cout << "| Stack  | Current    |" << std::endl;
    std::cout << "| Top    | Token      |" << std::endl;
    std::cout << "+--------+------------+" << std::endl;

    Token *curr_token = lex.getNextToken();

    while (!stk.empty()) {
        auto top = stk.top();

        std::cout << "stack top: " << top.token << std::endl;
        std::cout << "curr token: " << *(curr_token->getKey()) << std::endl;

        const bool is_match_or_error_handled = handleMatchOrError(top, curr_token);
        if (is_match_or_error_handled) {
            continue;
        }

        // Handle non-terminal at stack top
        handleNonTerminal(top, curr_token);
    }
    // Accept the grammar if the stack is empty
    std::cout << "Accept!" << std::endl;
}

bool PredictiveTopDownParser::handleMatchOrError(const StackItem &top, Token *&curr_token) {
    if (top.isTerminal) {
        if (top.token == *(curr_token->getKey())) {
            handleMatch(top, curr_token);
            return true;
        } else {
            handleMissingTerminal(top);
            return true;
        }
    }
    return false;
}

void PredictiveTopDownParser::handleMatch(const StackItem &top, Token *&curr_token) {
    std::cout << "Match " << *(curr_token->getKey()) << std::endl;
    stk.pop();
    curr_token = lex.getNextToken(); // Advance to the next token
}


void PredictiveTopDownParser::handleNonTerminal(const StackItem &top, Token *&curr_token) {
    const CellValue *cellValue = predictive_table.lookUp(top.token, *(curr_token->getKey()));
    ParsingTableEntryType entryType = cellValue->getPredictiveTableEntryType();

    switch (entryType) {
        case ParsingTableEntryType::EMPTY:
            handleEmptyEntry(top, curr_token);
            break;
        case ParsingTableEntryType::SYNC:
            handleSyncEntry(top);
            break;
        case ParsingTableEntryType::VALID_PRODUCTION:
            handleValidProduction(top, cellValue);
            break;
    }
}

// Helper functions for error handling and stack operations
void PredictiveTopDownParser::handleMissingTerminal(const StackItem &top) {
    std::cerr << "Error: missing " << top.token << ", discarded" << std::endl;
    stk.pop();
}

void PredictiveTopDownParser::handleEmptyEntry(const StackItem &top, Token *&curr_token) {
    std::cerr << "Error:(illegal " << top.token << ") at line("
        << curr_token->getPosition()->line_number << ") column("
        << curr_token->getPosition()->column_number << ") – discard " << curr_token << std::endl;
    curr_token = lex.getNextToken();
}

void PredictiveTopDownParser::handleSyncEntry(const StackItem &top) {
    std::cerr << "Error: missing " << top.token << ", discarded" << std::endl;
    stk.pop();
}

void PredictiveTopDownParser::handleValidProduction(const StackItem &top, const CellValue *cellValue) {
    stk.pop();
    auto production = cellValue->getProduction().productions[0];
    setNextDerivation(top, production);
    pushProductionToStack(production);
}

void PredictiveTopDownParser::pushProductionToStack(const std::vector<std::string> &production) {
    for (auto it = production.rbegin(); it != production.rend(); ++it) {
        bool isTerminal = non_terminals.find(*it) == non_terminals.end();
        stk.push({*it, isTerminal});
    }
    if (stk.top().token == EPSILON) {
        stk.pop();
    }
}

/**
 * @param top should be the non-terminal at the top of the stack
 * @param curr_production is the production to be added to the next derivation
 * @brief construct next derivation from last derivation and curr_production
 * */
void PredictiveTopDownParser::setNextDerivation(
        const StackItem &top, std::vector<std::string> &curr_production) {

    std::vector<std::string> next_derivation;
    auto last_derivation = left_most_derivation.back();

    auto it = last_derivation.begin();
    for (; it != last_derivation.end(); ++it) {
        if (*it == top.token) {
            ++it;
            break;
        }
        next_derivation.push_back(*it);
    }

    for (auto &symbol: curr_production) {
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

    int max_length = 0;  // Find the longest sentence for alignment
    for (const auto &production: left_most_derivation) {
        max_length = std::max(max_length, static_cast<int>(production.size()));
    }

    for (const auto &production: left_most_derivation) {
        // Use std::setw to align the sentences
        for (const auto &sentential: production) {
            std::cout << std::setw(max_length) << std::left << sentential << " ";
        }
        std::cout << std::endl;
    }
}

void PredictiveTopDownParser::generateMarkdownLeftmostDerivation(const std::string& filename) {
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    outputFile << "\nLeftmost derivation:\n\n";

    int max_length = 0;
    for (const auto &production: left_most_derivation) {
        max_length = std::max(max_length, static_cast<int>(production.size()));
    }

    // Print table header
    outputFile << "| Step |";
    for (int i = 1; i <= max_length; ++i) {
        outputFile << " Symbol " << i << " |";
    }
    outputFile << "\n";

    // Print separator line
    outputFile << "| --- |";
    for (int i = 1; i <= max_length; ++i) {
        outputFile << " --- |";
    }
    outputFile << "\n";

    // Print table rows
    for (size_t i = 0; i < left_most_derivation.size(); ++i) {
        outputFile << "| " << i + 1 << " |";
        for (const auto &symbol : left_most_derivation[i]) {
            outputFile << " " << std::setw(max_length) << std::left << symbol << " |";
        }
        outputFile << "\n";
    }

    outputFile.close();
}