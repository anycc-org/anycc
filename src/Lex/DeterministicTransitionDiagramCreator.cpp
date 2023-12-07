#include "Lex/NFAState.h"
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
    std::set<const NFAState*> start_states = transdig->getRecursiveEpsilonClosure(start_state);
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
                std::set<const NFAState*> result_next_states;
                std::set<const NFAState*> next_states = transdig->getAllNextStates(current_states, c);
                for(auto s : next_states) {
                    std::set<const NFAState*> closure_next_states = transdig->getRecursiveEpsilonClosure(s);
                    for(auto s2 : closure_next_states) {
                        result_next_states.insert(s2);
                    }
                }
                if(visited.find(result_next_states) == visited.end()) {
                    queue.push(result_next_states);
                }
                new_table[current_states][c] = result_next_states;
            }
        }
    }
    std::vector<const NFAState*> new_end_states;
    const NFAState* new_start_state = transdig->mergeStates(new_table, start_state, transdig->getEndStates(), new_end_states, transdig->getInputs());
    transdig->clear();
    transdig->fillTable(new_start_state, new_end_states);
    return transdig;
}