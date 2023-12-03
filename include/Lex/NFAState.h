#pragma once
#include <unordered_map>
#include <vector>
#include <string>

class NFAState {
public:
    NFAState();
    ~NFAState();
    NFAState(const NFAState& other, std::unordered_map<int, NFAState*>& copiedStates);

    void addTransition(char c, NFAState *state);
    std::unordered_map<char, std::vector<NFAState*>> getTransitions() const;
    void setTransitions(std::unordered_map<char, std::vector<NFAState*>> trans);
    int getStateId() const;
    std::string getTokenName() const;
    void setTokenName(const std::string& name);
    bool isEndState() const;
    void printState() const;
private:
    static int id;
    int stateId;
    std::string tokenName;
    std::unordered_map<char, std::vector<NFAState*>> transitions;
};
