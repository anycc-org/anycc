#include "Analyzer.h"

Analyzer::Analyzer(std::string &program_file_name, NFAState *start_state, TransitionDiagram *transition_diagram)
        : symbol_table(SymbolTable::getInstance()) {
    this->program_file_name = program_file_name;
    this->start_state = start_state;
    this->transition_diagram = transition_diagram;

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
        if (file->eof()) {
            acceptTokenAndRecoverErrorIfExists(acceptanceState, buffer);
            break;
        } else if (c == '\n') {
            acceptTokenAndRecoverErrorIfExists(acceptanceState, buffer);

            buffer = "";
            state = this->start_state;
            line_number++;
            i = 0;
        } else if (c == ' ') {
            acceptTokenAndRecoverErrorIfExists(acceptanceState, buffer);

            buffer = "";
            state = this->start_state;
            i++;
        } else {
            state = getNextState(c, state);
            buffer += c;

            if (state->isEndState()) {
                acceptanceState = {state, {buffer, line_number, i - buffer.length()}};
            } else if (state->getTokenName() == "error") {
                if (acceptanceState.state == nullptr) {
                    std::cout << '\"' << buffer << '\"' << " is bad token at " << line_number + 1 << ":" << i << '\n';
                    buffer.clear();
                    i++;
                    continue;
                }

                acceptTokenAndRecoverErrorIfExists(acceptanceState, buffer);
                continue;
            }
            i++;
        }
    }
}

void Analyzer::acceptTokenAndRecoverErrorIfExists(AcceptanceStateEntry &acceptanceState, std::string &buffer) {
    addToken(acceptanceState.state, acceptanceState.word);
    buffer.erase(0, acceptanceState.word.lexeme.length());
    acceptanceState = {nullptr, {}};

    if (!buffer.empty())
        panicModeErrorRecovery(buffer);
}

const NFAState *Analyzer::getNextState(char &c, const NFAState *state) {
    auto next_states = transition_diagram->lookup(state, c);
    if (next_states.empty() || dead_states.find(next_states.at(0)) != dead_states.end()) {
        auto *empty_state = new NFAState();
        empty_state->setTokenName("error");
        return empty_state;
    }

    return next_states.at(0);
}

void Analyzer::panicModeErrorRecovery(std::string &buffer) {
    const NFAState *state = this->start_state;
    AcceptanceStateEntry acceptanceState = {nullptr, {}};
    int line_number = 0;
    std::size_t size = buffer.length();
    int i = 0;

    while (i < size) {
        char c = buffer[i];
        state = getNextState(c, state);
        buffer += c;

        if (state->isEndState()) {
            acceptanceState = {state, {buffer, line_number, i - buffer.length()}};
        } else if (state->getTokenName() == "error") {
            if (acceptanceState.state == nullptr) {
                std::cout << '\"' << buffer << '\"' << " is bad token at " << line_number + 1 << ":" << i << '\n';
                buffer.clear();
                i++;
                continue;
            }

            acceptTokenAndRecoverErrorIfExists(acceptanceState, buffer);
            continue;
        }
        i++;
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

