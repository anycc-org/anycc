#include "Lex/NFAState.h"
#include "Lex/TransitionDiagram.h"
#include "Lex/DeterministicTransitionDiagramCreator.h"
#include <queue>
#include "Lex/Epsilon.h"

TransitionDiagram *
DeterministicTransitionDiagramCreator::subsetConstruction(TransitionDiagram *transition_diagram, bool inplace) {
    if (inplace) return subsetConstructionInplace(transition_diagram);
    return subsetConstructionInplace(
            new TransitionDiagram(transition_diagram->getStartState(), std::vector<const NFAState *>(
                                          transition_diagram->getEndStates().begin(), transition_diagram->getEndStates().end()),
                                  transition_diagram->getTokens(),
                                  transition_diagram->getTokensPriority()));
}

TransitionDiagram *
DeterministicTransitionDiagramCreator::subsetConstructionInplace(TransitionDiagram *transition_diagram) {
    const NFAState *start_state = transition_diagram->getStartState();
    std::queue<std::set<const NFAState *>> queue;
    std::set<std::set<const NFAState *>> visited;
    std::set<const NFAState *> start_states = transition_diagram->getRecursiveEpsilonClosure(start_state);
    queue.push(start_states);
    std::map<std::set<const NFAState *>, std::map<char, std::set<const NFAState *>>> new_table;
    while (!queue.empty()) {
        std::set<const NFAState *> current_states = queue.front();
        queue.pop();
        if (visited.find(current_states) != visited.end()) continue;
        visited.insert(current_states);
        new_table[current_states] = std::map<char, std::set<const NFAState *>>();
        for (auto c: transition_diagram->getInputs()) {
            if (c != EPSILON) {
                std::set<const NFAState *> result_next_states;
                std::set<const NFAState *> next_states = transition_diagram->getAllNextStates(current_states, c);
                for (auto s: next_states) {
                    std::set<const NFAState *> closure_next_states = transition_diagram->getRecursiveEpsilonClosure(s);
                    for (auto s2: closure_next_states) {
                        result_next_states.insert(s2);
                    }
                }
                if (visited.find(result_next_states) == visited.end()) {
                    queue.push(result_next_states);
                }
                new_table[current_states][c] = result_next_states;
            }
        }
    }
    std::vector<const NFAState *> new_end_states;
    std::unordered_map<const NFAState *, std::string> new_end_states_tokens_map;
    const NFAState *new_start_state = TransitionDiagram::mergeStates(transition_diagram, new_table, new_end_states,
                                                                     new_end_states_tokens_map);
    transition_diagram->clear();
    transition_diagram->fillTable(new_start_state, new_end_states, transition_diagram->getTokens(),
                                  new_end_states_tokens_map, false);
    return transition_diagram;
}