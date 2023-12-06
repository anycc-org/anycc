#include "Lex/NFAState.h"
#include "Lex/TransitionDiagram.h"
#include <Lex/TransitionDiagramMinimizer.h>


TransitionDiagram* TransitionDiagramMinimizer::minimize(TransitionDiagram* transdig, bool inplace) {
    if(inplace) return minimizeInplace(transdig);
    return minimizeInplace(new TransitionDiagram(transdig->getStartState(), std::vector<const NFAState*>(transdig->getEndStates().begin(), transdig->getEndStates().end())));
}

TransitionDiagram* TransitionDiagramMinimizer::minimizeInplace(TransitionDiagram* transdig) {
    const NFAState* start_state = transdig->getStartState();
    std::vector<std::vector<std::unordered_set<const NFAState*>>> all_sets;
    all_sets.push_back(std::vector<std::unordered_set<const NFAState*>>());
    all_sets[0].push_back(std::unordered_set<const NFAState*>(transdig->getNotEndStates()));
    all_sets[0].push_back(std::unordered_set<const NFAState*>(transdig->getEndStates()));
    std::vector<std::unordered_set<const NFAState*>> prev_sets = all_sets[0];
    while(true) {
        auto equi_table = this->constructEquivelanceTable(transdig, prev_sets);
        std::vector<std::unordered_set<const NFAState*>> new_sets;
        for(auto& set : prev_sets) {
            auto result_new_sets = this->constructNewEqivelanceSets(set, equi_table); 
            for(auto& new_set : result_new_sets) new_sets.push_back(new_set); 
        }
        all_sets.push_back(new_sets);
        if(new_sets.size() == prev_sets.size()) break;
        prev_sets = new_sets;
    }
    std::map<std::vector<const NFAState*>,  std::map<char, std::vector<const NFAState*>>> new_table;
    for(auto& set_current_states : all_sets[all_sets.size() - 1]){
        std::vector<const NFAState*> current_states(set_current_states.begin(), set_current_states.end());
        new_table[current_states] = std::map<char, std::vector<const NFAState*>>();
        for(auto c : transdig->getInputs()) {
            if(c != 'e') {
                std::vector<const NFAState*> next_states = transdig->getAllNextStates(current_states, c);
                next_states = this->extractNewMergedStatesFromOld(next_states[0], all_sets[all_sets.size() - 1]);
                if(next_states.size() > 0) {
                    new_table[current_states][c] = next_states;
                }
                else {
                    new_table[current_states][c] = std::vector<const NFAState*>();
                }
            }
        }
    }
    std::vector<const NFAState*> new_end_states;
    const NFAState* new_start_state = TransitionDiagram::mergeStates(new_table, start_state, transdig->getEndStates(), new_end_states, transdig->getInputs());
    transdig->clear();
    transdig->fillTable(new_start_state, new_end_states);
    return transdig;
}

std::unordered_map<const NFAState*, std::vector<size_t>> TransitionDiagramMinimizer::constructEquivelanceTable(TransitionDiagram* transdig, std::vector<std::unordered_set<const NFAState*>>& sets) {
    std::unordered_map<const NFAState*, std::vector<size_t>> equi_table;
    for(size_t i = 0; i < sets.size(); i++) {
        auto& set = sets[i];
        for(auto& state : set) {
            std::vector<size_t> sets_nums(set.size());
            for(auto c : transdig->getInputs()) {
                auto states_vec = transdig->lookup(state, c);
                long long index = this->getSetIndex(states_vec[0], sets);
                if(index != -1) sets_nums.push_back(index);
            }
            equi_table[state] = sets_nums;
        }
    }
    return equi_table;
}

long long TransitionDiagramMinimizer::getSetIndex(const NFAState* state, std::vector<std::unordered_set<const NFAState*>>& sets) {
    long long result = -1;
    for(size_t i = 0; i < sets.size(); i++) {
        if(sets[i].find(state) != sets[i].end()) return i;
    }
    return result;
}

std::vector<std::unordered_set<const NFAState*>> TransitionDiagramMinimizer::constructNewEqivelanceSets(std::unordered_set<const NFAState*>& set, std::unordered_map<const NFAState*, std::vector<size_t>>& table) {
    std::vector<std::unordered_set<const NFAState*>> new_sets;
    std::map<std::vector<size_t>, std::unordered_set<const NFAState*>> sets_map;
    for(auto& kv : table) {
        if(set.find(kv.first) != set.end()) {
            if(sets_map.find(kv.second) == sets_map.end()) {
                sets_map[kv.second] = std::unordered_set<const NFAState*>();
            }
            sets_map[kv.second].insert(kv.first);
        }
    }   
    for(auto& kv : sets_map) {
        new_sets.push_back(kv.second);
    }
    return new_sets;
}

std::vector<const NFAState*> TransitionDiagramMinimizer::extractNewMergedStatesFromOld(const NFAState* state, std::vector<std::unordered_set<const NFAState*>> states) {
    for(auto& set : states) {
        if(set.find(state) != set.end()) return std::vector<const NFAState*>(set.begin(), set.end());
    }
    return std::vector<const NFAState*>();
}