#include "Lex/TransitionDiagram.h"
#include <Lex/DeterministicTransitionDiagramCreator.h>
#include <queue>

TransitionDiagram* DeterministicTransitionDiagramCreator::subsetConstruction(TransitionDiagram* transdig, bool inplace) {
    if(inplace) return subsetConstructionInplace(transdig);
    return subsetConstructionInplace(new TransitionDiagram(transdig->getStartState(), std::vector<const NFAState*>(transdig->getEndStates().begin(), transdig->getEndStates().end())));
}

TransitionDiagram* DeterministicTransitionDiagramCreator::subsetConstructionInplace(TransitionDiagram* transdig) {
    const NFAState* start_state = transdig->getStartState();
    std::queue<std::vector<const NFAState*>> queue;
    std::set<std::vector<const NFAState*>> visited;
    std::vector<const NFAState*> start_vector;
    start_vector.push_back(transdig->getStartState());
    queue.push(std::vector<const NFAState*>(start_vector));
    std::map<std::vector<const NFAState*>,  std::map<char, std::vector<const NFAState*>>> new_table;
    while(!queue.empty()) {
        std::vector<const NFAState*> current_states = queue.front();
        queue.pop();
        if(visited.find(current_states) != visited.end()) continue;
        visited.insert(current_states);
        new_table[current_states] = std::map<char, std::vector<const NFAState*>>();
        for(auto c : transdig->getInputs()) {
            if(c != 'e') {
                std::vector<const NFAState*> next_states = transdig->getAllNextStates(current_states, c);
                if(next_states.size() > 0) {
                    if(visited.find(next_states) == visited.end()) {
                        queue.push(next_states);
                        new_table[current_states][c] = next_states;
                    }
                }
                else {
                    new_table[current_states][c] = std::vector<const NFAState*>();
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