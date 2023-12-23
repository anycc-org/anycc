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

    AcceptanceStateEntry() = default;
};

class Analyzer {
public:
    Analyzer(std::string &program_file_name, const NFAState *start_state, TransitionDiagram *transition_diagram);

    ~Analyzer();

    /**
     * @brief Get the next token
     * @warning the responsibility of deleting pointer is the caller's
     * @return Pointer to the next token
     */
    Token *getNextToken();

    /**
     * @brief Print the symbol table
     */
    void printSymbolTable();

private:
    std::string program_file_name;
    std::unordered_map<const NFAState *, std::string> end_states_tokens_map;
    std::unordered_set<const NFAState *> dead_states;
    std::unordered_set<const NFAState *> final_states;
    std::unordered_set<char> inputs;
    std::queue<Token *> tokens;
    const NFAState *start_state;
    TransitionDiagram *transition_diagram;
    std::ifstream *file;
    SymbolTable &symbol_table;
    AcceptanceStateEntry acceptance_state;
    const NFAState *current_state;
    std::string buffer;
    int line_number;
    std::size_t column_number;
    char c{};
    bool is_dead_state{};

    /**
     * @brief Accept the given token
     * @param acceptanceState The acceptance state of the token
     * @param buffer The buffer of the token
     * @warning the responsibility of deleting pointer is the caller's
     */
    const NFAState *getNextState(char &terminal, const NFAState *state);

    /**
     * @brief Add the given token to the tokens queue and add it to the symbol table if it's id
     * @param acceptanceState The acceptance state of the token
     * @param buffer The buffer of the token
     */
    Token *addToken(const NFAState *state, Word &word);

    /**
     * @brief Accept the given token, call addToken and reset the acceptance state and delete it from buffer
     * @param acceptance_state The acceptance state of the token
     * @param buffer The buffer of the token
     */
    Token *acceptToken();

    /**
     * @brief Check if the given state is a acceptance state
     * @param state The state to be checked
     * @return True if the given state is a acceptance state, false otherwise
     */
    bool isAcceptanceState(const NFAState *state);

    /**
     * @brief Log the given error
     * @param line_number The line number of the error
     * @param i The index of the error
     * @param c The character of the error
     */
    static void logError(int line_number, size_t i, std::string &c);

    /**
     * @brief Maximal munch algorithm with error recovery
     * @param line_number The line number of the error
     * @param column_number The index of the error
     * @param acceptanceState The acceptance state of the token
     * @param state The state of the token
     * @param buffer The buffer of the token
     * @param bypass True if the buffer got space, new line or eof then no more characters can be found and any errors should be printed, false otherwise
     */
    Token *maximalMunchWithErrorRecovery(bool bypass);

    /**
     * @brief Check if the given state is a dead state
     * @param state The state to be checked
     * @return True if the given state is a dead state, false otherwise
     */
    static bool isDeadState(const NFAState *state);
};