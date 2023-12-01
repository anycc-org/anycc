#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include "NFAState.h"

class NFA {
public:
    NFA();
    NFA(NFAState* start, NFAState* end);
    ~NFA();

    NFAState* getStartState() const;
    NFAState* getEndState() const;
    static NFA* basicCharToNFA(char c);
    static NFA* wordToNFA(const std::string& word);
    static NFA* unionNAFs(NFA* nfa1, NFA* nfa2);
    static NFA* concatNAFs(NFA* nfa1, NFA* nfa2);
    static NFA* kleeneStarNFA(NFA* nfa);
    static NFA* positiveClosureNFA(NFA* nfa);
    static NFA* unionRangeNFAs(NFA* rangeStartNFA, NFA* rangeEndNFA);
    void printNFA() const;

private:
    NFAState* startState;
    NFAState* endState;
};