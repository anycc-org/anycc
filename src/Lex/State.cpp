#include <Lex/State.h>
#include <stack>
#include <iostream>

void State::printState () const {
    
    // visualize the NFA using dfs
    std::unordered_map<int, bool> visited;
    std::stack<State*> stack;
    stack.push(const_cast<State*>(this));

    while (!stack.empty()) {
        State* currentState = stack.top();
        stack.pop();
        visited[currentState->getStateId()] = true;
        std::cout << currentState->getStateId();

        for (auto& transition : currentState->getTransitions()) {
            std::cout << "  on(" << transition.first << ") -> ";
            for (State* state : transition.second) {
                std::cout << state->getStateId() << " ";
                if (visited.find(state->getStateId()) == visited.end()) {
                    stack.push(state);
                }
            }
        }
        std::cout << std::endl;
    }
}