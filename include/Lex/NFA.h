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
    NFA(const NFA& other) noexcept;
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

    
    /**
     * @brief removes all epsilon
      transitions from the NFA and return a pointer to epslion-free NFA
     * @warning incase of inplace was false, the deletion of the returned NFA Pointer is the responsibilty of 
     the caller  
     * @param nfa the NFA to remove epsilon transisitons from
     * @param inplace if true the same pointer passed will be returned, no new object created, otherwise, a new NFA will be created. 
     * @return NFA* NFA pointer , see warning
     */
    static NFA* removeEpsilonTransitions(NFA* nfa, bool inplace=false);
    
    void printNFA() const;

private:
    NFAState* startState;
    NFAState* endState;


};