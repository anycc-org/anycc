#include "Analyzer.h"
#include "Epsilon.h"

Analyzer::Analyzer(std::string &program_file_name, const NFAState *start_state, TransitionDiagram *transition_diagram)
        : symbol_table(SymbolTable::getInstance()) {
    this->program_file_name = program_file_name;
    file = new std::ifstream(this->program_file_name);
    this->start_state = start_state;
    this->transition_diagram = transition_diagram;
    this->dead_states = transition_diagram->getDeadStates();
    this->final_states = transition_diagram->getEndStates();

    std::vector<char> inputs_vector = transition_diagram->getInputs();
    this->inputs = std::unordered_set<char>(inputs_vector.begin(), inputs_vector.end());
    this->end_states_tokens_map = transition_diagram->getEndStatesTokensMap();
    this->tokens = std::queue<Token *>();

    this->acceptance_state = {nullptr, {}};
    this->current_state = this->start_state;
    this->buffer = "";
    this->line_number = 0;
    this->column_number = 0;
    this->is_dead_state = false;
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

Token *Analyzer::getNextTokenInQueue() {
    if (tokens.empty())
        return nullptr;

    auto next_token = tokens.front();
    tokens.pop();
    return next_token;
}

Token *Analyzer::getNextToken() {
    if (!buffer.empty()) {
        auto token = maximalMunchWithErrorRecovery(false);
        if (is_dead_state) {
            is_dead_state = false;
            column_number++;
        }
        if (token != nullptr) return token;
    }

    while (file->get(c)) {
        // If the character is not a new line or a space and not in the inputs set
        // then it is a bad token, and we should log an error and accept the last accepted token
        if (c != '\t' && c != '\n' && c != ' ' && inputs.find(c) == inputs.end()) {
            auto token = acceptToken();
            std::string s(1, c);
            logError(line_number, column_number, s);
            current_state = this->start_state;
            column_number++;
            if (token != nullptr) {
                return token;
            }
        } else if (c == '\n') {
            Token *token = nullptr;
            if (acceptance_state.state != nullptr) {
                token = acceptToken();
            }

            if (!buffer.empty()) {
                token = maximalMunchWithErrorRecovery(true);

                if (token == nullptr && acceptance_state.state != nullptr) {
                    token = acceptToken();
                }
            }

            buffer = "";
            current_state = this->start_state;
            line_number++;
            column_number = 0;
            if (token != nullptr) return token;
        } else if (c == ' ' || c == '\t') {
            Token *token = nullptr;
            if (acceptance_state.state != nullptr) {
                token = acceptToken();
            }

            if (!buffer.empty()) {
                token = maximalMunchWithErrorRecovery(true);

                if (token == nullptr && acceptance_state.state != nullptr) {
                    token = acceptToken();
                }
            }

            buffer = "";
            current_state = this->start_state;
            if (c == '\t') {
                column_number += 4;
            } else {
                column_number++;
            }
            if (token != nullptr) return token;
        } else {
            Token *token = nullptr;
            current_state = getNextState(c, current_state);
            buffer += c;

            if (isAcceptanceState(current_state)) {
                acceptance_state = {current_state,
                                    {buffer, line_number, column_number - (int) buffer.length() + 2}};
            } else if (isDeadState(current_state)) {
                // If the state is dead then we should log an error if there is no acceptance state
                if (acceptance_state.state == nullptr) {
                    std::string s(1, buffer[0]);
                    logError(line_number, column_number - 1, s);
                    buffer.erase(0, 1);
                    token = maximalMunchWithErrorRecovery(false);
                } else {
                    is_dead_state = true;
                    token = acceptToken();
                    if (token != nullptr) return token;
                    continue;
                }
            }
            column_number++;
            if (token != nullptr) return token;
        }
    }

    Token *token = nullptr;
    // Check if there is a token in the buffer or there is an acceptance state
    if (acceptance_state.state != nullptr) {
        token = acceptToken();
    } else {
        if (!buffer.empty()) {
            token = maximalMunchWithErrorRecovery(true);
        }
    }

    if (token != nullptr) return token;
    return new Token(new std::string("$"), new std::string("$"),
                     new Position(line_number, column_number));
}

Token *Analyzer::maximalMunchWithErrorRecovery(bool bypass) {
    current_state = start_state;
    int j = 0, prev_j = -1;
    while (j < buffer.length() && prev_j != j) {
        prev_j = j;
        char b = buffer[j++];
        current_state = getNextState(b, current_state);
        if (isAcceptanceState(current_state)) {
            acceptance_state = {current_state, {buffer.substr(0, j), line_number, column_number - j}};
        } else if (isDeadState(current_state) || bypass) {
            std::string temp_buffer = buffer;
            j = 0;
            if (bypass) {
                logError(line_number, column_number - j - 1, temp_buffer);
            }
            return acceptToken();
        }
    }
    return nullptr;
}

void Analyzer::logError(int line_number, size_t i, std::string &c) {
    std::cout << '\"' << c << '\"' << " is bad token at " << line_number + 1 << ":" << i + 1 << '\n';
}

bool Analyzer::isDeadState(const NFAState *state) { return state->getTokenName() == DEAD; }

bool Analyzer::isAcceptanceState(const NFAState *state) { return final_states.find(state) != final_states.end(); }

Token *Analyzer::acceptToken() {
    if (acceptance_state.state == nullptr)
        return nullptr;

    auto token = addToken(acceptance_state.state, acceptance_state.word);
    buffer.erase(0, acceptance_state.word.lexeme.length());
    acceptance_state = {nullptr, {}};
    current_state = start_state;

    return token;
}

const NFAState *Analyzer::getNextState(char &terminal, const NFAState *state) {
    auto next_states = transition_diagram->lookup(state, terminal);
    if (next_states.empty() || dead_states.find(next_states.at(0)) != dead_states.end()) {
        auto *empty_state = new NFAState();
        empty_state->setTokenName("dead");
        return empty_state;
    }

    return next_states.at(0);
}

Token *Analyzer::addToken(const NFAState *state, Word &word) {
    auto token_name = new std::string(end_states_tokens_map.at(state));
    auto token_id = state->getStateId();
    auto lexeme = new std::string(word.lexeme);
    auto position = new Position(word.line_number, word.column_number);
    auto token = new Token(token_name, lexeme, position);

    tokens.push(token);

    if (*token_name == "id")
        symbol_table.insertEntry(*lexeme, *token_name, token_id, word.line_number + 1, word.column_number);

    return token;
}

void Analyzer::printSymbolTable() {
    symbol_table.printTable();
}
