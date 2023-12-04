#pragma once
#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include "Lex/NFA.h"
#include "Lex/NFAState.h"


class TransitionDiagram {
public:
    TransitionDiagram(const NFAState* start_state, std::vector<const NFAState*> end_states);
    ~TransitionDiagram() = default;
    std::vector<char> getInputs() const;
    std::vector<const NFAState*> getStates() const;
    const NFAState* getStartState() const;
    std::vector<const NFAState*> getEndStates() const;
    std::vector<const NFAState*> getDeadStates() const;
    std::vector<const NFAState*> lookup(const NFAState* state, char input); 
    std::vector<const NFAState*> lookup(int state_id, char input); 
    std::unordered_map<char, std::vector<const NFAState*>> lookup(const NFAState* state);
    std::unordered_map<char, std::vector<const NFAState*>> lookup(int state_id); 
    std::vector<const NFAState*> getRecursiveEpsilonClosure(const NFAState* state);
    std::vector<const NFAState*> getAllNextStates(std::vector<const NFAState*>& states, char input);
    
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


    /**
     * @brief do subset construction on
      the Transition Diagram and return a pointer to the  Transition Diagram
     * @warning incase of inplace was false, the deletion of the returned Transition Diagram Pointer is the responsibilty of 
     the caller  
     * @param inplace if true the same pointer passed will be returned, no new object created, otherwise, a new Transition Diagram will be created. 
     * @return TransitionDiagram* Transition Diagram pointer , see warning
     */
    TransitionDiagram* subsetConstruction(bool inplace=true);


      /**
     * @brief minimize
      the Transition Diagram and return a pointer to the Transition Diagram
     * @warning incase of inplace was false, the deletion of the returned Transition Diagram Pointer is the responsibilty of 
     the caller  
     * @param inplace if true the same pointer passed will be returned, no new object created, otherwise, a new Transition Diagram will be created. 
     * @return TransitionDiagram* Transition Diagram pointer , see warning
     */
    TransitionDiagram* minimize(bool inplace=true);

    void print() const;
    
private:
    std::unordered_map<const NFAState*, std::unordered_map<char, std::vector<const NFAState*>>> table;
    std::unordered_map<int, const NFAState*> state_id_map;
    std::unordered_set<char> inputs;
    std::unordered_set<const NFAState*> states;
    std::unordered_set<const NFAState*> dead_states;
    std::unordered_set<const NFAState*> end_states;
    std::unordered_map<const NFAState*, std::vector<std::string>> end_states_tokens_map;
    const NFAState* startState;

    void fillTable(const NFAState* state, std::vector<const NFAState*> end_states);
    const NFAState* getStateId(int state_id);
    TransitionDiagram* removeEpsilonTransitionsInplace(TransitionDiagram* transdig);
    TransitionDiagram* subsetConstructionInplace(TransitionDiagram* transdig);
    TransitionDiagram* minimizeInplace(TransitionDiagram* transdig);
    static const NFAState* mergeStates(std::map<std::vector<const NFAState*>, std::map<char, std::vector<const NFAState*>>>& new_table, const NFAState* start_state, std::vector<const NFAState*>& new_end_states, std::vector<char> inputs);
    bool static isDeadState(const NFAState* state);
    bool static isDeadStateNew(std::vector<const NFAState*> states);
};