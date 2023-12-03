#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Lex/NFAState.h"


class TransitionDiagram {
public:
    TransitionDiagram(NFAState* start_state);
    ~TransitionDiagram() = default;
    std::vector<const NFAState*> lookup(const NFAState* state, char input); 
    std::vector<const NFAState*> lookup(int state_id, char input); 
    std::unordered_map<char, std::vector<const NFAState*>> lookup(const NFAState* state);
    std::unordered_map<char, std::vector<const NFAState*>> lookup(int state_id); 



private:
    std::unordered_map<int, const NFAState*> state_id_map;
    std::vector<char> inputs;
    std::unordered_map<const NFAState*, std::unordered_map<char, std::vector<const NFAState*>>> table;
    void fillTable(NFAState* state);
    const NFAState* getStateId(int state_id);
};