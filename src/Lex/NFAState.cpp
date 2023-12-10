#include <Lex/NFAState.h>
#include <iostream>
#include <utility>
#include <stack>
#include "Lex/Epsilon.h"

NFAState::NFAState() : stateId(id++) {}

NFAState::~NFAState() {
    // handle circular references while deleting NFAStates
    std::unordered_map<int, bool> visited;
    std::stack<NFAState*> stack;
    std::stack<NFAState*> deleteStack;
    stack.push(this);

    while (!stack.empty()) {
        NFAState* currentState = stack.top();
        stack.pop();
        deleteStack.push(currentState);

        // Check if the state has already been visited
        if (visited.find(currentState->getStateId()) != visited.end()) {
            continue;
        }

        visited[currentState->getStateId()] = true;

        for (auto& transition : currentState->getTransitions()) {
            for (NFAState* state : transition.second) {
                if (visited.find(state->getStateId()) == visited.end()) {
                    stack.push(state);
                }
            }
        }
    }
    while (deleteStack.size() > 1) {
        NFAState* curr = deleteStack.top(); deleteStack.pop();
        curr->transitions.clear();
    }
    visited.clear();
    deleteStack.pop();
    transitions.clear();
}

NFAState::NFAState(NFAState &other, std::unordered_map<NFAState*, NFAState*> &copiedStates) {
    stateId = id++;
    tokenName = other.tokenName;
    copiedStates[&other] = this;
    // Perform a deep copy of the transitions
    for (const auto& entry : other.transitions) { // entry: <char, vector<NFAState*>>
        char symbol = entry.first;
        const std::vector<NFAState*>& states = entry.second;
        for (NFAState* state : states) {
            if (copiedStates.find(state) == copiedStates.end()) {
                NFAState* stateCopy = new NFAState(*state, copiedStates);
                addTransition(symbol, stateCopy);
            }
            else {
                NFAState* stateCopy = copiedStates[state];
                addTransition(symbol, stateCopy);
            }
        }
    }
}

int NFAState::id = 1;

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

std::string NFAState::getTokenName() const { return tokenName; }

void NFAState::setTokenName(const std::string& name) { tokenName = name; }

void NFAState::printState() const {
    // visualize the NFA using dfs
    std::unordered_map<int, bool> visited;
    std::stack<NFAState*> stack;
    stack.push(const_cast<NFAState*>(this));

    while (!stack.empty()) {
        NFAState* currentState = stack.top();
        stack.pop();
        visited[currentState->getStateId()] = true;

        for (auto& transition : currentState->getTransitions()) {
            std::cout << currentState->getStateId() << "  on(" << transition.first << ") -> ";
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