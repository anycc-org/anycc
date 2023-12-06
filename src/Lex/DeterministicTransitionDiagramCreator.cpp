#include "Lex/TransitionDiagram.h"
#include <Lex/DeterministicTransitionDiagramCreator.h>
#include <queue>

TransitionDiagram* DeterministicTransitionDiagramCreator::subsetConstruction(TransitionDiagram* transdig, bool inplace) {
    if(inplace) return subsetConstructionInplace(transdig);
    return subsetConstructionInplace(new TransitionDiagram(transdig->getStartState(), std::vector<const NFAState*>(transdig->getEndStates().begin(), transdig->getEndStates().end())));
}

TransitionDiagram* DeterministicTransitionDiagramCreator::subsetConstructionInplace(TransitionDiagram* transdig) {
    const NFAState* start_state = transdig->getStartState();
    std::queue<std::set<const NFAState*>> queue;
    std::set<std::set<const NFAState*>> visited;
    std::set<const NFAState*> start_states;
    start_states.insert(transdig->getStartState());
    queue.push(start_states);
    std::map<std::set<const NFAState*>,  std::map<char, std::set<const NFAState*>>> new_table;
    while(!queue.empty()) {
        std::set<const NFAState*> current_states = queue.front();
        queue.pop();
        if(visited.find(current_states) != visited.end()) continue;
        visited.insert(current_states);
        new_table[current_states] = std::map<char, std::set<const NFAState*>>();
        for(auto c : transdig->getInputs()) {
            if(c != '#') {
                std::set<const NFAState*> next_states = transdig->getAllNextStates(current_states, c);
                bool self = false;
                // for(auto s : next_states) {
                //     for(auto s_temp : visited)
                //     if(current_states.find(s) != current_states.end()) {
                //         new_table[current_states][c] = current_states;
                //         queue.push(current_states);
                //         next_states = current_states;
                //         self = true;
                //         break;
                //     }
                // }
                if(self) continue;
                if(next_states.size() > 0) {
                    if(visited.find(next_states) == visited.end()) {
                        queue.push(next_states);
                        new_table[current_states][c] = next_states;
                    }
                }
                else {
                    new_table[current_states][c] = std::set<const NFAState*>();
                }
            }
        }
    }
    std::vector<const NFAState*> new_end_states;
    const NFAState* new_start_state = transdig->mergeStates(new_table, start_state, transdig->getEndStates(),  new_end_states, transdig->getInputs());
    transdig->clear();
    transdig->fillTable(new_start_state, new_end_states);
    return transdig;
}