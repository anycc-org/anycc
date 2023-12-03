#pragma once
#include <unordered_map>
#include <vector>
#include <Lex/State.h>


class NFAState : public State {
public:
    NFAState();

    NFAState(const NFAState& other, std::unordered_map<int, NFAState*>& copiedStates);


    void addTransition(char c, NFAState *state);
    std::unordered_map<char, std::vector<State*>> getTransitions() override;
    void setTransitions(std::unordered_map<char, std::vector<NFAState*>> trans);
    int getStateId() const override;
    bool isEndState() const override;

private:
    static int id;
    int stateId;
    std::unordered_map<char, std::vector<NFAState*>> transitions;
};
