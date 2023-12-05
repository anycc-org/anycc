#pragma once

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <set>
#include <queue>
#include "Rules.h"
#include "RuleType.h"
#include "Token.h"
#include "FileReader.h"
#include "TransitionDiagram.h"
#include "SymbolTable.h"

struct Word {
    std::string lexeme;
    int line_number;
};

struct RecoveryStateEntry {
    const NFAState *state;
    int leftPointer;
    int rightPointer;
};

class Analyzer : public FileReader {
public:
    Analyzer(std::string &program_file_name, NFAState *start_state, TransitionDiagram *transition_diagram);

    ~Analyzer();

    void analyzeProgram();

    /**
     * @brief Get the next token
     * @warning the responsibility of deleting pointer is the caller's
     * @return Pointer to the next token
     */
    Token *getNextToken();

private:
    std::string program_file_name;
    std::vector<Word> words;
    std::unordered_set<const NFAState *> dead_states;
    std::queue<Token *> tokens;
    NFAState *start_state;
    TransitionDiagram *transition_diagram;
    SymbolTable &symbol_table;

    void readProgram();

    void parseLine(std::string &line, int line_number) override;

    const NFAState *simulate(Word &word);

    void addToken(const NFAState *state, Word &word);

    void panicModeErrorRecovery(Word &word);
};