#include "Analyzer.h"

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
    // read characters until EOF with buffer string and line number and column number
    // get next state, save it in a variable if it is accepted state and continue reading
    // if next state is not end state, continue reading till space or end of line
    // if next state is dead state or empty, add last accepted state to tokens and recover from error on the remaining buffer

    AcceptanceStateEntry acceptanceState = {nullptr, {}};
    const NFAState *state = this->start_state;
    std::string buffer;
    int line_number = 0;
    std::size_t i = 0;
    char c;

    while (file->get(c)) {
        if (c != '\n' && c != ' ' && inputs.find(c) == inputs.end()) {
            acceptTokenAndRecoverErrorIfExists(acceptanceState, buffer);
            std::cout << '\"' << c << '\"' << " is bad token at " << line_number + 1 << ":" << i + 1 << '\n';
            i++;
            state = this->start_state;
            continue;
        }

        if (c == '\n') {
            if (acceptanceState.state == nullptr) {
                if (!buffer.empty()) {
                    state = this->start_state;
                    state = getNextState(c, state);
                    if (isFinalState(state))
                        acceptanceState = {state, {buffer, line_number, i - (int) buffer.length()}};
                    else if (state->getTokenName() == "dead") {
                        std::cout << '\"' << buffer << '\"' << " is bad token at " << line_number + 1 << ":"
                                  << i - buffer.length() + 1 << '\n';
                        buffer.clear();
                    }
                }
                buffer = "";
                state = this->start_state;
                line_number++;
                i = 0;
                continue;
            }

            acceptTokenAndRecoverErrorIfExists(acceptanceState, buffer);

            buffer = "";
            state = this->start_state;
            line_number++;
            i = 0;
        } else if (c == ' ') {
            if (acceptanceState.state == nullptr) {
                if (!buffer.empty()) {
                    state = this->start_state;
                    state = getNextState(c, state);
                    if (isFinalState(state))
                        acceptanceState = {state, {buffer, line_number, i - (int) buffer.length()}};
                    else if (state->getTokenName() == "dead") {
                        std::cout << '\"' << buffer << '\"' << " is bad token at " << line_number + 1 << ":"
                                  << i - buffer.length() + 1 << '\n';
                        buffer.clear();
                    }
                }
                buffer = "";
                state = this->start_state;
                i++;
                continue;
            }

            acceptTokenAndRecoverErrorIfExists(acceptanceState, buffer);

            buffer = "";
            state = this->start_state;
            i++;
        } else {
            state = getNextState(c, state);
            buffer += c;

            if (isFinalState(state)) {
                acceptanceState = {state, {buffer, line_number, i - (int) buffer.length()}};
            } else if (state->getTokenName() == "dead") {
                if (acceptanceState.state == nullptr) {
                    std::cout << '\"' << buffer[0] << '\"' << " is bad token at " << line_number + 1 << ":"
                              << i << "wodka" << '\n';
                    buffer.erase(0, 1);
                    state = this->start_state;
                    state = getNextState(c, state);
                    if (isFinalState(state))
                        acceptanceState = {state, {buffer, line_number, i - (int) buffer.length()}};
                    else if (state->getTokenName() == "dead") {
                        std::cout << '\"' << buffer << '\"' << " is bad token at " << line_number + 1 << ":"
                                  << i - buffer.length() + 1 << '\n';
                        buffer.clear();
                        state = this->start_state;
                    }
                    i++;
                    continue;
                }

                addToken(acceptanceState.state, acceptanceState.word);
                buffer.erase(0, acceptanceState.word.lexeme.length());
                acceptanceState = {nullptr, {}};

                if (!buffer.empty()) {
                    state = this->start_state;
                    state = getNextState(c, state);
                    if (isFinalState(state))
                        acceptanceState = {state, {buffer, line_number, i - (int) buffer.length()}};
                    else if (state->getTokenName() == "dead") {
                        std::cout << '\"' << buffer << '\"' << " is bad token at " << line_number + 1 << ":"
                                  << i - buffer.length() + 1 << '\n';
                        buffer.clear();
                        state = this->start_state;
                    }
                    i++;
                    continue;
                }

                continue;
            }
            i++;
        }
    }
    if (acceptanceState.state != nullptr) {
        acceptTokenAndRecoverErrorIfExists(acceptanceState, buffer);
        if (!buffer.empty()) {
            state = this->start_state;
            state = getNextState(c, state);
            if (isFinalState(state))
                acceptanceState = {state, {buffer, line_number, i - (int) buffer.length()}};
            else if (state->getTokenName() == "dead") {
                std::cout << '\"' << buffer << '\"' << " is bad token at " << line_number + 1 << ":"
                          << i - buffer.length() + 1 << '\n';
                buffer.clear();
            }
        }
    } else {
        if (!buffer.empty()) {
            state = this->start_state;
            state = getNextState(c, state);
            if (isFinalState(state))
                acceptanceState = {state, {buffer, line_number, i - (int) buffer.length()}};
            else {
                std::cout << '\"' << buffer << '\"' << " is bad token at " << line_number + 1 << ":"
                          << i - buffer.length() + 1 << '\n';
                buffer.clear();
            }
        }
    }
}

bool Analyzer::isFinalState(const NFAState *state) { return final_states.find(state) != final_states.end(); }

void Analyzer::acceptTokenAndRecoverErrorIfExists(AcceptanceStateEntry &acceptanceState, std::string &buffer) {
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

void Analyzer::panicModeErrorRecovery(std::string &buffer) {
    const NFAState *state = this->start_state;
    AcceptanceStateEntry acceptanceState = {nullptr, {}};
    int line_number = 0;
    std::string temp_buffer;
    std::size_t size = buffer.length();
    int i = 0;

    while (i < size) {
        char c = buffer[i];
        state = getNextState(c, state);
        temp_buffer += c;

        if (isFinalState(state)) {
            acceptanceState = {state, {temp_buffer, line_number, i - temp_buffer.length()}};
        } else if (state->getTokenName() == "error") {
            if (acceptanceState.state == nullptr) {
                std::cout << '\"' << temp_buffer << '\"' << " is bad token at " << line_number + 1 << ":" << i << '\n';
                buffer.clear();
                i++;
                continue;
            }

            acceptTokenAndRecoverErrorIfExists(acceptanceState, temp_buffer);
            continue;
        }
        i++;
    }
}

void Analyzer::addToken(const NFAState *state, Word &word) {
    auto token_name = new std::string(end_states_tokens_map.at(state));
    auto token_id = state->getStateId();
    auto lexeme = new std::string(word.lexeme);
    auto token = new Token(token_name, lexeme);

    tokens.push(token);

    if (*token_name == "id")
        symbol_table.insertEntry(*lexeme, *token_name, token_id, word.line_number);
}

