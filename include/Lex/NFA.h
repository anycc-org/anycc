#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include "NFAState.h"

class NFA {
public:
    NFA();
    ~NFA();

    NFAState* getStartState() const;
    NFAState* getEndState() const;
//    void setStartState(const NFAState& state);
//    void setEndState(const NFAState& state);
    static NFA* basicCharToNFA(char c);
    static NFA* unionNAFs(NFA* nfa1, NFA* nfa2);
    static NFA* concatNAFs(NFA* nfa1, NFA* nfa2);
    static NFA* kleeneStarNFA(NFA* nfa);
    static NFA* positiveClosureNFA(NFA* nfa);
    static NFA* unionRangeNFAs(NFA* rangeStartNFA, NFA* rangeEndNFA);
    void printNFA() const;

private:
    NFAState *startState;
    NFAState *endState;

    NFA constructDigitNFA(NFA& startDigitNFA);
    NFA constructLowerCaseNFA(NFA& startLetterNFA);
    NFA constructUpperCaseNFA(NFA& startLetterNFA);
};