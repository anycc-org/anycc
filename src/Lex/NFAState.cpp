#include <Lex/NFAState.h>
#include <iostream>
#include <utility>
#include <stack>

int NFAState::id = 1;

NFAState::NFAState() : stateId(id++) {}

NFAState::NFAState(const NFAState &other, std::unordered_map<int, NFAState *> &copiedStates) {
    stateId = other.stateId;
    copiedStates[stateId] = this;
    // Perform a deep copy of the transitions
    for (const auto& entry : other.transitions) { // entry: <char, vector<NFAState*>>
        char symbol = entry.first;
        const std::vector<NFAState*>& states = entry.second;
        for (NFAState* state : states) {
            if (copiedStates.find(state->stateId) == copiedStates.end()) {
                NFAState* stateCopy = new NFAState(*state, copiedStates);
                addTransition(symbol, stateCopy);
            }
            else {
                NFAState* stateCopy = copiedStates[state->stateId];
                addTransition(symbol, stateCopy);
            }
        }
    }
}

void NFAState::addTransition(char c, NFAState* state) {
    transitions[c].push_back(state);
}

std::unordered_map<char, std::vector<NFAState*>> NFAState::getTransitions() const {
    return transitions;
}

void NFAState::setTransitions(std::unordered_map<char, std::vector<NFAState *>> trans) {
    // transfers the ownership of the map
    transitions = std::move(trans);
}

int NFAState::getStateId() const { return stateId; }

bool NFAState::isEndState() const {
    return transitions.empty();
}

void NFAState::printState() const {
    // visualize the NFA using dfs
    std::unordered_map<int, bool> visited;
    std::stack<NFAState*> stack;
    stack.push(const_cast<NFAState*>(this));

    while (!stack.empty()) {
        NFAState* currentState = stack.top();
        stack.pop();
        visited[currentState->getStateId()] = true;
        std::cout << currentState->getStateId();

        for (auto& transition : currentState->getTransitions()) {
            std::cout << "  on(" << transition.first << ") -> ";
            for (NFAState* state : transition.second) {
                std::cout << state->getStateId() << " ";
                if (visited.find(state->getStateId()) == visited.end()) {
                    stack.push(state);
                }
            }
        }
        std::cout << std::endl;
    }
}