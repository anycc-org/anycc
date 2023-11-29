#include <Lex/NFAState.h>
#include <iostream>

int NFAState::id = 1;

NFAState::NFAState() : stateId(id++) {}

void NFAState::addTransition(char c, NFAState* state) {
    transitions[c].push_back(state);
}

std::unordered_map<char, std::vector<NFAState*>> NFAState::getTransitions() const {
    return transitions;
}

int NFAState::getStateId() const { return stateId; }

void NFAState::printState() const {
    std::cout << "State: " << stateId << " : {  ";
    for (auto& transition : transitions) {
        std::cout << "Transition on " << transition.first << " to: ";
        for (auto& state : transition.second) {
            std::cout << state->getStateId() << " ";
        }
        std::cout << ", ";
    }
}