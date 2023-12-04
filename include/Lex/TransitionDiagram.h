#pragma once
#include <iostream>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include "Lex/NFA.h"
#include "Lex/NFAState.h"


class TransitionDiagram {
public:
    TransitionDiagram(const NFAState* start_state);
    ~TransitionDiagram() = default;
    std::vector<char> getInputs();
    std::vector<const NFAState*> getStates();
    const NFAState* getStartState();
    const NFAState* getEndState();
    std::vector<const NFAState*> lookup(const NFAState* state, char input); 
    std::vector<const NFAState*> lookup(int state_id, char input); 
    std::unordered_map<char, std::vector<const NFAState*>> lookup(const NFAState* state);
    std::unordered_map<char, std::vector<const NFAState*>> lookup(int state_id); 
    std::vector<const NFAState*> getEpsilonClosure(const NFAState* state);
    std::vector<const NFAState*> getAllNextStates(std::vector<const NFAState*> states, char input);
    /**
     * @brief create a new NFA using the transition diagram
     * @warning the responsbility of deleting pointer is the caller's 
     * @return NFA* pointer to the NFA createdd
     */
    NFA* createNFAFromTable();

    /**
     * @brief removes all epsilon
      transitions from the Transition Diagram and return a pointer to epslion-free Transition Diagram
     * @warning incase of inplace was false, the deletion of the returned Transition Diagram Pointer is the responsibilty of 
     the caller  
     * @param inplace if true the same pointer passed will be returned, no new object created, otherwise, a new Transition Diagram will be created. 
     * @return TransitionDiagram* Transition Diagram pointer , see warning
     */
    TransitionDiagram* removeEpsilonTransitions(bool inplace=true);
    TransitionDiagram* subsetConstruction(bool inplace=true);

    void print() const;
    
private:
    std::unordered_map<const NFAState*, std::unordered_map<char, std::vector<const NFAState*>>> table;
    std::unordered_map<int, const NFAState*> state_id_map;
    std::unordered_set<char> inputs;
    std::unordered_set<const NFAState*> states;
    const NFAState* startState;
    const NFAState* endtState;
    void fillTable(const NFAState* state);
    const NFAState* getStateId(int state_id);
    TransitionDiagram* removeEpsilonTransitionsInplace(TransitionDiagram* transdig);
    TransitionDiagram* subsetConstructionInplace(TransitionDiagram* transdig);

};