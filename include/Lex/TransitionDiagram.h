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
    TransitionDiagram(const NFAState *start_state, std::vector<const NFAState *> end_states,
                      std::vector<std::string> tokens, std::unordered_map<std::string, int> tokensPriority);

    ~TransitionDiagram();

    std::vector<char> getInputs() const;

    const NFAState *getStartState();

    void fillTable(const NFAState *state, std::vector<const NFAState *> end_states_vector,
                   std::vector<std::string> tokens_vector,
                   std::unordered_map<const NFAState *, std::string> states_tokens_map, bool new_fill);

    std::unordered_set<const NFAState *> getStates();

    std::unordered_set<const NFAState *> getEndStates();

    std::unordered_set<const NFAState *> getDeadStates();

    std::unordered_set<const NFAState *> getNotEndStates();

    std::unordered_set<const NFAState *> getNotDeadStates();

    std::unordered_set<const NFAState *> getNotEndAndDeadStates();

    std::vector<std::string> getTokens();

    std::unordered_map<std::string, int> getTokensPriority();

    std::unordered_map<const NFAState *, std::string> getEndStatesTokensMap();

    std::vector<const NFAState *> lookup(const NFAState *state, char input);

    std::vector<const NFAState *> lookup(int state_id, char input);

    std::unordered_map<char, std::vector<const NFAState *>> lookup(const NFAState *state);

    std::unordered_map<char, std::vector<const NFAState *>> lookup(int state_id);

    std::set<const NFAState *> getRecursiveEpsilonClosure(const NFAState *state);

    std::set<const NFAState *> getAllNextStates(const std::set<const NFAState *> &states_set, char input);

    const static NFAState *mergeStates(TransitionDiagram *transition_diagram,
                                       std::map<std::set<const NFAState *>, std::map<char, std::set<const NFAState *>>> &new_table,
                                       std::vector<const NFAState *> &new_end_states,
                                       std::unordered_map<const NFAState *, std::string> &new_end_states_tokens_map);

    static bool
    isEndStateNew(const std::set<const NFAState *> &states, std::unordered_set<const NFAState *> end_states);

    void clear();

    void toDotFile(const std::string &file_name);

    void toCSVFile(const std::string &file_name);

    void toMDFile(const std::string &file_name);

private:
    std::unordered_map<const NFAState *, std::unordered_map<char, std::vector<const NFAState *>>> table;
    std::unordered_map<int, const NFAState *> state_id_map;
    std::vector<char> inputs;
    const NFAState *startState;
    std::unordered_set<const NFAState *> states;
    std::unordered_set<const NFAState *> end_states;
    std::unordered_set<const NFAState *> dead_states;
    std::unordered_map<const NFAState *, std::string> end_states_tokens_map;
    std::vector<std::string> tokens;
    std::unordered_map<std::string, int> tokensPriority;

    const NFAState *getStateId(int state_id);

    bool static isDeadState(const NFAState *state, std::unordered_set<const NFAState *> end_states);

    std::string getMaxPriorityToken(const std::vector<std::string> &tokens_vector);
};