#include "Analyzer.h"

Analyzer::Analyzer(std::string &program_file_name, NFAState *start_state, TransitionDiagram *transition_diagram)
        : symbol_table(SymbolTable::getInstance()) {
    this->program_file_name = program_file_name;
    this->start_state = start_state;
    this->transition_diagram = transition_diagram;
    this->words = std::vector<Word>();

    auto dead_states_vector = transition_diagram->getDeadStates();
    this->dead_states = std::unordered_set<const NFAState *>(dead_states_vector.begin(), dead_states_vector.end());
    this->tokens = std::queue<Token *>();
}

Analyzer::~Analyzer() {
    delete start_state;
    delete transition_diagram;

    while (!tokens.empty()) {
        auto token = tokens.front();
        tokens.pop();
        delete token;
    }
}

void Analyzer::analyzeProgram() {
    readProgram();
    for (auto &word: words) {
        std::cout << word.lexeme << '\n';
        auto state = simulate(word);
        if (state->isEndState()) {
            addToken(state, word);
            std::cout << "Token: " << state->getTokenName() << '\n';
        } else {
            std::cout << "Error\n";
            panicModeErrorRecovery(word);
        }
    }
}

Token *Analyzer::getNextToken() {
    if (tokens.empty())
        return nullptr;

    auto next_token = tokens.front();
    tokens.pop();
    return next_token;
}

void Analyzer::readProgram() {
    auto *file = new std::ifstream(this->program_file_name);
    readFile(file);
}

void Analyzer::parseLine(std::string &line, int line_number) {
    std::string delimiter = " ";
    size_t pos;
    std::string token;

    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        Word word = {token, line_number};
        words.push_back(word);
        line.erase(0, pos + delimiter.length());
    }
}

const NFAState *Analyzer::simulate(Word &word) {
    const NFAState *state = start_state;

    for (char c: word.lexeme) {
        auto next_states = transition_diagram->lookup(state, c);
        if (next_states.empty() || dead_states.find(next_states.at(0)) != dead_states.end())
            return {};

        auto next_state = next_states.at(0);

        state = next_state;
    }

    return state;
}

void Analyzer::panicModeErrorRecovery(Word &word) {
    RecoveryStateEntry recoveryState = {nullptr, 0, 0};
    auto *state = this->start_state;
    int leftPointer = 0;
    int rightPointer = 0;
    std::size_t size = word.lexeme.length();

    while (true) {
        if (rightPointer < size) {
            char c = word.lexeme[rightPointer];
            auto next_states = transition_diagram->lookup(state, c);
            if (next_states.empty() || dead_states.find(next_states.at(0)) != dead_states.end()) {
                if (recoveryState.rightPointer == recoveryState.leftPointer) {
                    std::cout << " " << word.lexeme.substr(recoveryState.leftPointer) << " "
                              << "is bad token at line number:"
                              << word.line_number + 1 << '\n';
                    return;
                }

                auto lexeme = word.lexeme.substr(recoveryState.leftPointer, recoveryState.rightPointer + 1);
                Word new_word = {lexeme, word.line_number};
                addToken(recoveryState.state, word);

                leftPointer = recoveryState.rightPointer + 1;
                rightPointer = recoveryState.rightPointer + 1;

                recoveryState = {};
                state = this->start_state;
            } else {
                auto next_state = next_states.at(0);
                if (next_state->isEndState())
                    recoveryState = {next_state, leftPointer, rightPointer};
                rightPointer++;
            }
        } else {
            if (recoveryState.rightPointer == recoveryState.leftPointer) {
                std::cout << " " << word.lexeme << " " << "is bad token at line number:" << word.line_number + 1
                          << '\n';
                return;
            } else {
                auto lexeme = word.lexeme.substr(recoveryState.leftPointer, recoveryState.rightPointer + 1);
                Word new_word = {lexeme, word.line_number};
                addToken(recoveryState.state, word);

                leftPointer = recoveryState.rightPointer + 1;
                rightPointer = recoveryState.rightPointer + 1;

                recoveryState = {};
                state = this->start_state;
            }
        }
    }
}

void Analyzer::addToken(const NFAState *state, Word &word) {
    auto token_name = state->getTokenName();
    auto token_id = state->getStateId();
    auto *token = new Token(&token_name, &word.lexeme);

    tokens.push(token);

    if (token_name == "id")
        symbol_table.insertEntry(word.lexeme, token_name, token_id, word.line_number);
}

