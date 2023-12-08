#include "Analyzer.h"
#include "Epsilon.h"

Analyzer::Analyzer(std::string &program_file_name, const NFAState *start_state, TransitionDiagram *transition_diagram)
        : symbol_table(SymbolTable::getInstance()) {
    this->program_file_name = program_file_name;
    this->start_state = start_state;
    this->transition_diagram = transition_diagram;
    this->dead_states = transition_diagram->getDeadStates();
    this->final_states = transition_diagram->getEndStates();

    std::vector<char> inputs_vector = transition_diagram->getInputs();
    this->inputs = std::unordered_set<char>(inputs_vector.begin(), inputs_vector.end());
    this->end_states_tokens_map = transition_diagram->getEndStatesTokensMap();
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
    symbol_table.printTable();
}

void Analyzer::analyzeProgram() {
    readProgram();
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

void Analyzer::readTemplate(std::ifstream *file) {
    AcceptanceStateEntry acceptanceState = {nullptr, {}};
    const NFAState *state = this->start_state;
    std::string buffer;
    int line_number = 0;
    std::size_t i = 0;
    char c;

    while (file->get(c)) {
        if (c != '\n' && c != ' ' && inputs.find(c) == inputs.end()) {
            acceptToken(acceptanceState, buffer);
            std::string s(1, c);
            logError(line_number, i, s);
            state = this->start_state;
            i++;
        } else if (c == '\n') {
            if (acceptanceState.state != nullptr)
                acceptToken(acceptanceState, buffer);

            if (!buffer.empty()) {
                maximalMunchWithErrorRecovery(line_number, i, acceptanceState, state, buffer, c, true);
                if (acceptanceState.state != nullptr)
                    acceptToken(acceptanceState, buffer);
            }

            buffer = "";
            state = this->start_state;
            line_number++;
            i = 0;
        } else if (c == ' ') {
            if (acceptanceState.state != nullptr)
                acceptToken(acceptanceState, buffer);

            if (!buffer.empty()) {
                maximalMunchWithErrorRecovery(line_number, i, acceptanceState, state, buffer, c, true);
                if (acceptanceState.state != nullptr)
                    acceptToken(acceptanceState, buffer);
            }

            buffer = "";
            state = this->start_state;
            i++;
        } else {
            state = getNextState(c, state);
            buffer += c;

            if (isFinalState(state)) {
                acceptanceState = {state, {buffer, line_number, (int) i - (int) buffer.length() + 2}};
            } else if (isDeadState(state)) {
                if (acceptanceState.state == nullptr) {
                    std::string s(1, buffer[0]);
                    logError(line_number, i - 1, s);
                    buffer.erase(0, 1);
                    maximalMunchWithErrorRecovery(line_number, i, acceptanceState, state, buffer, c);
                    i++;
                    continue;
                }

                acceptToken(acceptanceState, buffer);

                if (!buffer.empty()) {
                    maximalMunchWithErrorRecovery(line_number, i, acceptanceState, state, buffer, c);
                    i++;
                }

                continue;
            }
            i++;
        }
    }

    if (acceptanceState.state != nullptr) {
        acceptToken(acceptanceState, buffer);
        if (!buffer.empty())
            maximalMunchWithErrorRecovery(line_number, i, acceptanceState, state, buffer, c);
    } else {
        if (!buffer.empty())
            maximalMunchWithErrorRecovery(line_number, i, acceptanceState, state, buffer, c, true);
    }
}

bool Analyzer::isDeadState(const NFAState *state) { return state->getTokenName() == DEAD; }

void Analyzer::maximalMunchWithErrorRecovery(int line_number, size_t i, AcceptanceStateEntry &acceptanceState,
                                             const NFAState *&state, std::string &buffer, char &c, bool bypass) {
    state = start_state;
    int j = 0, prev_j = -1;
    while (j < buffer.length() && prev_j != j) {
        prev_j = j;
        char b = buffer[j++];
        state = getNextState(b, state);
        if (isFinalState(state)) {
            acceptanceState = {state, {buffer.substr(0, j), line_number, (int) i - j}};
        } else if (isDeadState(state) || bypass) {
            acceptToken(acceptanceState, buffer);
            state = start_state;
            j = 0;
            if (bypass) {
                logError(line_number, i - j - 1, buffer);
            }
        }

    }
}

void Analyzer::logError(int line_number, size_t i, std::string &c) {
    std::cout << '\"' << c << '\"' << " is bad token at " << line_number + 1 << ":" << i + 1 << '\n';
}

bool Analyzer::isFinalState(const NFAState *state) { return final_states.find(state) != final_states.end(); }

void Analyzer::acceptToken(AcceptanceStateEntry &acceptanceState, std::string &buffer) {
    if (acceptanceState.state == nullptr)
        return;

    addToken(acceptanceState.state, acceptanceState.word);
    buffer.erase(0, acceptanceState.word.lexeme.length());
    acceptanceState = {nullptr, {}};
}

const NFAState *Analyzer::getNextState(char &c, const NFAState *state) {
    auto next_states = transition_diagram->lookup(state, c);
    if (next_states.empty() || dead_states.find(next_states.at(0)) != dead_states.end()) {
        auto *empty_state = new NFAState();
        empty_state->setTokenName("dead");
        return empty_state;
    }

    return next_states.at(0);
}

void Analyzer::addToken(const NFAState *state, Word &word) {
    auto token_name = new std::string(end_states_tokens_map.at(state));
    auto token_id = state->getStateId();
    auto lexeme = new std::string(word.lexeme);
    auto token = new Token(token_name, lexeme);

    tokens.push(token);

    if (*token_name == "id")
        symbol_table.insertEntry(*lexeme, *token_name, token_id, word.line_number + 1, word.column_number);
}

