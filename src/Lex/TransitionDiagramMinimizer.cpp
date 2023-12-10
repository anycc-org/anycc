#include "Lex/NFAState.h"
#include "Lex/TransitionDiagram.h"
#include "Lex/TransitionDiagramMinimizer.h"
#include "Lex/Epsilon.h"


TransitionDiagram *TransitionDiagramMinimizer::minimize(TransitionDiagram *transition_diagram, bool inplace) {
    if (inplace) return minimizeInplace(transition_diagram);
    return minimizeInplace(new TransitionDiagram(transition_diagram->getStartState(),
                                                 std::vector<const NFAState *>(
                                                         transition_diagram->getEndStates().begin(),
                                                         transition_diagram->getEndStates().end()),
                                                 transition_diagram->getTokens(),
                                                 transition_diagram->getTokensPriority()));
}

TransitionDiagram *TransitionDiagramMinimizer::minimizeInplace(TransitionDiagram *transition_diagram) {
    std::vector<std::vector<std::set<const NFAState *>>> all_sets;
    all_sets.push_back(std::vector<std::set<const NFAState *>>());
    all_sets[0].push_back(std::set<const NFAState *>(
            std::set<const NFAState *>(transition_diagram->getNotEndAndDeadStates().begin(),
                                       transition_diagram->getNotEndAndDeadStates().end())));
    for (const auto &token: transition_diagram->getTokens()) {
        std::set<const NFAState *> set;
        for (const auto &kv: transition_diagram->getEndStatesTokensMap()) {
            if (kv.second == token) {
                set.insert(kv.first);
            }
        }
        all_sets[0].push_back(set);
    }
    all_sets[0].push_back(std::set<const NFAState *>(
            std::set<const NFAState *>(transition_diagram->getDeadStates().begin(),
                                       transition_diagram->getDeadStates().end())));
    std::vector<std::set<const NFAState *>> prev_sets = all_sets[0];
    while (true) {
        auto equivalence_table = TransitionDiagramMinimizer::constructEquivalenceTable(transition_diagram, prev_sets);
        std::vector<std::set<const NFAState *>> new_sets;
        for (auto set: prev_sets) {
            auto result_new_sets = TransitionDiagramMinimizer::constructNewEquivalenceSets(set, equivalence_table);
            for (const auto &new_set: result_new_sets) new_sets.push_back(new_set);
        }
        all_sets.push_back(new_sets);
        if (new_sets.size() == prev_sets.size()) break;
        prev_sets = new_sets;
    }
    std::map<std::set<const NFAState *>, std::map<char, std::set<const NFAState *>>> new_table;
    for (auto &set_current_states: all_sets[all_sets.size() - 1]) {
        std::set<const NFAState *> ordered_set_current_states = std::set<const NFAState *>(set_current_states.begin(),
                                                                                           set_current_states.end());
        new_table[ordered_set_current_states] = std::map<char, std::set<const NFAState *>>();
        for (auto c: transition_diagram->getInputs()) {
            if (c != EPSILON) {
                std::set<const NFAState *> result_next_states;
                std::set<const NFAState *> next_states = transition_diagram->getAllNextStates(
                        ordered_set_current_states, c);
                for (auto s: next_states) {
                    std::set<const NFAState *> new_next_states = TransitionDiagramMinimizer::extractNewMergedStatesFromOld(
                            s, all_sets[
                                    all_sets.size() - 1]);
                    for (auto s2: new_next_states) {
                        result_next_states.insert(s2);
                    }
                }
                new_table[ordered_set_current_states][c] = result_next_states;
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

std::unordered_map<const NFAState *, std::vector<size_t>>
TransitionDiagramMinimizer::constructEquivalenceTable(TransitionDiagram *transition_diagram,
                                                      std::vector<std::set<const NFAState *>> &sets) {
    std::unordered_map<const NFAState *, std::vector<size_t>> equivalence_table;
    for (size_t i = 0; i < sets.size(); i++) {
        auto set = sets[i];
        for (auto state: set) {
            std::vector<size_t> sets_nums;
            for (auto c: transition_diagram->getInputs()) {
                if (c != EPSILON) {
                    auto states_vec = transition_diagram->lookup(state, c);
                    long long index = TransitionDiagramMinimizer::getSetIndex(states_vec[0], sets);
                    if (index != -1) sets_nums.push_back(index);
                }
            }
            equivalence_table[state] = sets_nums;
        }
    }
    return equivalence_table;
}

long long
TransitionDiagramMinimizer::getSetIndex(const NFAState *state, std::vector<std::set<const NFAState *>> &sets) {
    long long result = -1;
    for (long long i = 0; i < sets.size(); i++) {
        if (sets[i].find(state) != sets[i].end()) return i;
    }
    return result;
}

std::vector<std::set<const NFAState *>>
TransitionDiagramMinimizer::constructNewEquivalenceSets(std::set<const NFAState *> &set,
                                                        std::unordered_map<const NFAState *, std::vector<size_t>> &table) {
    std::vector<std::set<const NFAState *>> new_sets;
    std::map<std::vector<size_t>, std::set<const NFAState *>> sets_map;
    for (auto &kv: table) {
        if (set.find(kv.first) != set.end()) {
            if (sets_map.find(kv.second) == sets_map.end()) {
                sets_map[kv.second] = std::set<const NFAState *>();
            }
            sets_map[kv.second].insert(kv.first);
        }
    }
    new_sets.reserve(sets_map.size());
    for (auto &kv: sets_map) {
        new_sets.push_back(kv.second);
    }
    return new_sets;
}

std::set<const NFAState *> TransitionDiagramMinimizer::extractNewMergedStatesFromOld(const NFAState *state,
                                                                                     std::vector<std::set<const NFAState *>> states) {
    for (auto &set: states) {
        if (set.find(state) != set.end()) return {set.begin(), set.end()};
    }
    return {};
}