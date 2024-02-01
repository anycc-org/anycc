#ifndef ANYCC_NFASTATE_H
#define ANYCC_NFASTATE_H

#include <unordered_map>
#include <vector>
#include <string>

class NFAState {
public:
    NFAState();

    ~NFAState();

    NFAState(NFAState &other, std::unordered_map<NFAState *, NFAState *> &copiedStates);

    void addTransition(char c, NFAState *state);

    std::unordered_map<char, std::vector<NFAState *>> getTransitions() const;

    void setTransitions(std::unordered_map<char, std::vector<NFAState *>> trans);

    int getStateId() const;

    std::string getTokenName() const;

    void setTokenName(const std::string &name);

    void setEndState(bool isEnd);

    bool isEndState() const;

    void printState() const;

private:
    static int id;
    int stateId;
    bool endState = false;
    std::string tokenName;
    std::unordered_map<char, std::vector<NFAState *>> transitions;
};

#endif //ANYCC_NFASTATE_H