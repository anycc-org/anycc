#pragma once

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <set>
#include <queue>
#include <utility>
#include "Rules.h"
#include "RuleType.h"
#include "Token.h"
#include "FileReader.h"
#include "TransitionDiagram.h"
#include "SymbolTable.h"

struct Word {
    std::string lexeme;
    int line_number;
    int column_number;
};

struct AcceptanceStateEntry {
    const NFAState *state{};
    Word word;

    AcceptanceStateEntry(const NFAState *state, Word word) : state(state), word(std::move(word)) {}
};

class Analyzer : public FileReader {
public:
    Analyzer(std::string &program_file_name, const NFAState *start_state, TransitionDiagram *transition_diagram);

    ~Analyzer();

    void analyzeProgram();

    /**
     * @brief Get the next token
     * @warning the responsibility of deleting pointer is the caller's
     * @return Pointer to the next token
     */
    Token *getNextToken();

    void printSymbolTable();

    void readTemplate(std::ifstream *file) override;

private:
    std::string program_file_name;
    std::unordered_map<const NFAState *, std::string> end_states_tokens_map;
    std::unordered_set<const NFAState *> dead_states;
    std::unordered_set<const NFAState *> final_states;
    std::unordered_set<char> inputs;
    std::queue<Token *> tokens;
    const NFAState *start_state;
    TransitionDiagram *transition_diagram;
    SymbolTable &symbol_table;

    void readProgram();

    const NFAState *getNextState(char &c, const NFAState *state);

    void addToken(const NFAState *state, Word &word);

    void acceptToken(AcceptanceStateEntry &acceptanceState, std::string &buffer);

    bool isFinalState(const NFAState *state);

    static void logError(int line_number, size_t i, std::string &c);

    void
    maximalMunchWithErrorRecovery(int line_number, size_t i, AcceptanceStateEntry &acceptanceState,
                                  const NFAState *&state,
                                  std::string &buffer, char &c, bool bypass = false);

    static bool isDeadState(const NFAState *state);
};