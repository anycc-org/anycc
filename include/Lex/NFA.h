#ifndef ANYCC_NFA_H
#define ANYCC_NFA_H

#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include "NFAState.h"

class NFA {
public:
    NFA();

    NFA(NFAState *start, NFAState *end);

    NFA(const NFA &other) noexcept;

    ~NFA();

    NFAState *getStartState() const;

    NFAState *getEndState() const;

    std::vector<const NFAState *> getEndStates();

    std::string getTokenName() const;

    void setTokenName(const std::string &name);

    void addEndState(NFAState *state);

    static NFA *basicCharToNFA(char c);

    static NFA *wordToNFA(const std::string &word);

    static NFA *unionNAFs(NFA *nfa1, NFA *nfa2);

    static NFA *concatNAFs(NFA *nfa1, NFA *nfa2);

    static NFA *kleeneStarNFA(NFA *nfa);

    static NFA *positiveClosureNFA(NFA *nfa);

    static NFA *unionRangeNFAs(NFA *rangeStartNFA, NFA *rangeEndNFA);

    void printNFA() const;

private:
    std::string tokenName;
    NFAState *startState;
    NFAState *endState;
    std::vector<NFAState *> endStates;
};

#endif //ANYCC_NFA_H