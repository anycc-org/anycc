#pragma once
#include <unordered_map>
#include <vector>

class NFAState {
public:
    NFAState();

    void addTransition(char c, NFAState *state);

    std::unordered_map<char, std::vector<NFAState*>> getTransitions() const;
    void setTransitions(std::unordered_map<char, std::vector<NFAState*>> trans);

    int getStateId() const;

    void printState() const;
private:
    static int id;
    const int stateId;
    std::unordered_map<char, std::vector<NFAState*>> transitions;
};
