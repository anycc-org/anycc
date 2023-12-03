#include "Lex/State.h"
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

std::unordered_map<char, std::vector<State*>> NFAState::getTransitions() {
    return std::unordered_map<char, std::vector<State*>>(transitions.begin(), transitions.end());
}

void NFAState::setTransitions(std::unordered_map<char, std::vector<NFAState *>> trans) {
    // transfers the ownership of the map
    transitions = std::move(trans);
}

int NFAState::getStateId() const { return stateId; }

bool NFAState::isEndState() const {
    return transitions.empty();
}
