#include "Lex/NFAState.h"
#include "Lex/TransitionDiagram.h"
#include <Lex/TransitionDiagramMinimizer.h>


TransitionDiagram* TransitionDiagramMinimizer::minimize(TransitionDiagram* transdig, bool inplace) {
    if(inplace) return minimizeInplace(transdig);
    return minimizeInplace(new TransitionDiagram(transdig->getStartState(), std::vector<const NFAState*>(transdig->getEndStates().begin(), transdig->getEndStates().end())));
}

TransitionDiagram* TransitionDiagramMinimizer::minimizeInplace(TransitionDiagram* transdig) {
    const NFAState* start_state = transdig->getStartState();
    std::vector<std::vector<std::set<const NFAState*>>> all_sets;
    all_sets.push_back(std::vector<std::set<const NFAState*>>());
    all_sets[0].push_back(std::set<const NFAState*>(std::set<const NFAState*>(transdig->getNotEndStates().begin(), transdig->getNotEndStates().end())));
    all_sets[0].push_back(std::set<const NFAState*>(std::set<const NFAState*>(transdig->getEndStates().begin(), transdig->getEndStates().end())));
    std::vector<std::set<const NFAState*>> prev_sets = all_sets[0];
    while(true) {
        auto equi_table = this->constructEquivelanceTable(transdig, prev_sets);
        std::cout << prev_sets.size() << " oreved\n";
        std::vector<std::set<const NFAState*>> new_sets;
        for(auto set : prev_sets) {
            auto result_new_sets = this->constructNewEqivelanceSets(set, equi_table);
            for(auto new_set : result_new_sets) new_sets.push_back(new_set); 
        }
        all_sets.push_back(new_sets);
        if(new_sets.size() == prev_sets.size()) break;
        prev_sets = new_sets;
    }
    for(auto s : all_sets[all_sets.size() - 1]) {
        std::cout << s.size() << "\n";
        for(auto s2 : s) {
            std::cout << s2->getStateId() << " ";
        }
        std::cout << "\n";
    }
    std::map<std::set<const NFAState*>,  std::map<char, std::set<const NFAState*>>> new_table;
    for(auto& set_current_states : all_sets[all_sets.size() - 1]){
        std::set<const NFAState*> ordered_set_current_states = std::set<const NFAState*>(set_current_states.begin(), set_current_states.end());
        new_table[ordered_set_current_states] = std::map<char, std::set<const NFAState*>>();
        for(auto c : transdig->getInputs()) {
            if(c != '#') {
                std::set<const NFAState*> result_next_states;
                std::set<const NFAState*> next_states = transdig->getAllNextStates(ordered_set_current_states, c);
                for(auto s : next_states) {
                    std::set<const NFAState*> new_next_states = this->extractNewMergedStatesFromOld(s, all_sets[all_sets.size() - 1]);
                    for(auto s2 : new_next_states) {
                        result_next_states.insert(s2);
                    }
                }
                new_table[ordered_set_current_states][c] = result_next_states;
            }
        }
    }
    std::vector<const NFAState*> new_end_states;
    const NFAState* new_start_state = TransitionDiagram::mergeStates(new_table, start_state, transdig->getEndStates(), new_end_states, transdig->getInputs());
    transdig->clear();
    transdig->fillTable(new_start_state, new_end_states);
    return transdig;
}

std::unordered_map<const NFAState*, std::vector<size_t>> TransitionDiagramMinimizer::constructEquivelanceTable(TransitionDiagram* transdig, std::vector<std::set<const NFAState*>>& sets) {
    std::unordered_map<const NFAState*, std::vector<size_t>> equi_table;
    for(size_t i = 0; i < sets.size(); i++) {
        auto& set = sets[i];
        for(auto& state : set) {
            std::vector<size_t> sets_nums;
            for(auto c : transdig->getInputs()) {
                if(c != '#') {
                    auto states_vec = transdig->lookup(state, c);
                    long long index = this->getSetIndex(states_vec[0], sets);
                    if(index != -1) sets_nums.push_back(index);
                }
            }
            equi_table[state] = sets_nums;
        }
    }
    return equi_table;
}

long long TransitionDiagramMinimizer::getSetIndex(const NFAState* state, std::vector<std::set<const NFAState*>>& sets) {
    long long result = -1;
    for(size_t i = 0; i < sets.size(); i++) {
        if(sets[i].find(state) != sets[i].end()) return i;
    }
    return result;
}

std::vector<std::set<const NFAState*>> TransitionDiagramMinimizer::constructNewEqivelanceSets(std::set<const NFAState*>& set, std::unordered_map<const NFAState*, std::vector<size_t>>& table) {
    std::vector<std::set<const NFAState*>> new_sets;
    std::map<std::vector<size_t>, std::set<const NFAState*>> sets_map;
    for(auto& kv : table) {
        if(set.find(kv.first) != set.end()) {
            if(sets_map.find(kv.second) == sets_map.end()) {
                sets_map[kv.second] = std::set<const NFAState*>();
            }
            sets_map[kv.second].insert(kv.first);
        }
    }   
    for(auto& kv : sets_map) {
        new_sets.push_back(kv.second);
    }
    return new_sets;
}

std::set<const NFAState*> TransitionDiagramMinimizer::extractNewMergedStatesFromOld(const NFAState* state, std::vector<std::set<const NFAState*>> states) {
    for(auto& set : states) {
        if(set.find(state) != set.end()) return std::set<const NFAState*>(set.begin(), set.end());
    }
    return std::set<const NFAState*>();
}