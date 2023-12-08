#include "Lex/NFA.h"
#include "Lex/NFAState.h"
#include <Lex/TransitionDiagram.h>
#include <cstddef>
#include <map>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include "Lex/Epsilon.h"

TransitionDiagram::TransitionDiagram(const NFAState* start_state, std::vector<const NFAState*> end_states, std::vector<std::string> tokens, std::unordered_map<std::string, int> tokensPriority) {
    this->fillTable(start_state, end_states, tokens, std::unordered_map<const NFAState*, std::string>(), true);
    this->tokensPriority = tokensPriority;
}

/**
 * @brief Destroy the Transition Diagram:: Transition Diagram object
 * @todo should be implemented to delete newly created NFAStates usnig mergestates static method
 */
TransitionDiagram::~TransitionDiagram() {
}

std::vector<char> TransitionDiagram::getInputs() const {
    return this->inputs;
}

std::unordered_set<const NFAState*> TransitionDiagram::getStates() {
    return this->states;
}

const NFAState* TransitionDiagram::getStartState() {
    return this->startState;
}

std::unordered_set<const NFAState*> TransitionDiagram::getEndStates() {
    return this->end_states;
}

std::unordered_set<const NFAState*> TransitionDiagram::getDeadStates() {
    return this->dead_states;
}

std::unordered_set<const NFAState*> TransitionDiagram::getNotEndStates() {
    std::unordered_set<const NFAState*> res_set;
    for(auto state : this->getStates()) {
        if(this->getEndStates().find(state) == this->getEndStates().end()) res_set.insert(state);
    }
    return res_set;
}

std::unordered_set<const NFAState*> TransitionDiagram::getNotDeadStates() {
    std::unordered_set<const NFAState*> res_set;
    for(auto state : this->getStates()) {
        if(this->getDeadStates().find(state) == this->getDeadStates().end()) res_set.insert(state);
    }
    return res_set;
}

std::unordered_set<const NFAState*> TransitionDiagram::getNotEndAndDeadStates() {
    std::unordered_set<const NFAState*> res_set;
    for(auto state : this->getStates()) {
        if(this->getDeadStates().find(state) == this->getDeadStates().end() && 
           this->getEndStates().find(state) == this->getEndStates().end()) res_set.insert(state);
    }
    return res_set;
}

std::vector<std::string>  TransitionDiagram::getTokens() {
    return this->tokens;
}
std::unordered_map<const NFAState*, std::string> TransitionDiagram::getEndStatesTokensMap() {
    return this->end_states_tokens_map;
}
std::unordered_map<std::string, int> TransitionDiagram::getTokensPriority() {
    return this->tokensPriority;
}

std::vector<const NFAState*> TransitionDiagram::lookup(const NFAState* state, char input) {
    if(table.find(state) == table.end()) return std::vector<const NFAState*>();
    if(table[state].find(input) == table[state].end()) return std::vector<const NFAState*>();
    return this->table[state][input];
}

std::vector<const NFAState*> TransitionDiagram::lookup(int state_id, char input) {
    const NFAState* state = getStateId(state_id);
    if(state == nullptr) return std::vector<const NFAState*>();
    return this->lookup(state, input);
}

std::unordered_map<char, std::vector<const NFAState*>> TransitionDiagram::lookup(const NFAState* state) {
    if(table.find(state) == table.end()) return std::unordered_map<char, std::vector<const NFAState*>>();
    return this->table[state];
}

std::unordered_map<char, std::vector<const NFAState*>> TransitionDiagram::lookup(int state_id) {
    const NFAState* state = getStateId(state_id);
    if(state == nullptr) return std::unordered_map<char, std::vector<const NFAState*>>();
    return this->lookup(state);
}

std::set<const NFAState*> TransitionDiagram::getRecursiveEpsilonClosure(const NFAState* state) {
    std::stack<const NFAState*> stack;
    std::set<const NFAState*> visited;
    stack.push(state);
    while(!stack.empty()) {
        const NFAState* current_state = stack.top();
        stack.pop();
        visited.insert(current_state);
        std::vector<const NFAState*> next_states = this->lookup(current_state, EPSILON);
        for(auto next_state : next_states) {
            if(visited.find(next_state) == visited.end()) {
                stack.push(next_state);
            }
        }
    }
    return visited;
}

std::set<const NFAState*> TransitionDiagram::getAllNextStates(std::set<const NFAState*> states, char input) {
    std::set<const NFAState*> next_states;
    for(auto state : states) {
        std::vector<const NFAState*> next_stats = this->lookup(state, input);
        for(auto new_next_state : next_stats) {
            next_states.insert(new_next_state);
        }
    }
    return next_states;
}

/**
 * @todo create NFA from the table
 *  
 */
NFA* TransitionDiagram::createNFAFromTable() {
    return nullptr;
}

TransitionDiagram* TransitionDiagram::removeEpsilonTransitions(bool inplace) {
    if(inplace) return removeEpsilonTransitionsInplace(this);
    return removeEpsilonTransitionsInplace(new TransitionDiagram(this->getStartState(), std::vector<const NFAState*>(this->getEndStates().begin(), this->getEndStates().end()), this->tokens, this->getTokensPriority()));
}

TransitionDiagram* TransitionDiagram::removeEpsilonTransitionsInplace(TransitionDiagram* transdig) {
    std::vector<char> inputs = transdig->getInputs();
    std::vector<const NFAState*> states = std::vector<const NFAState*>(transdig->getStates().begin(), transdig->getStates().end());
    for(auto state : states) {
        std::set<const NFAState*> closure_states = transdig->getRecursiveEpsilonClosure(state);
        for(auto c : inputs) {
            std::unordered_set<const NFAState*> result_states;
            if(c != EPSILON) {
                std::set<const NFAState*> next_states = transdig->getAllNextStates(closure_states, c);
                for(auto tmp_state : next_states) {
                    std::set<const NFAState*> closure_next_state = transdig->getRecursiveEpsilonClosure(tmp_state);
                    for(auto next_state : closure_next_state) {
                        result_states.insert(next_state);
                    }
                }
                transdig->table[state][c] = std::vector<const NFAState*>(result_states.begin(), result_states.end());
            }
        }
    }
    for(auto state : states) {
        transdig->table[state].erase(EPSILON);
    }
    // transdig->inputs.erase(EPSILON);
    return transdig;
}

void TransitionDiagram::print() const {
    for(auto kv : this->table) {
        if(kv.first == this->startState) std::cout << "->";
        if(this->dead_states.find(kv.first) != this->dead_states.end()) std::cout << "x";
        if(this->end_states.find(kv.first) != this->end_states.end()) std::cout << "*";
        std::cout << kv.first->getStateId() << "\n";
        for(auto kv2 : kv.second) {
            std::cout << kv2.first << "\n";
            for(auto state : kv2.second) {
                std::cout << state->getStateId() << " ";
            }
            std::cout << "\n";
        }
        std::cout << "===============================================\n";
    }
}

void TransitionDiagram::clear() {
    // for(auto& state : this->states) delete state;
    this->inputs.clear();
    this->states.clear();
    this->table.clear();
    this->state_id_map.clear();
    this->end_states.clear();
    this->dead_states.clear();
}


void TransitionDiagram::toDotFile(std::string file_name) {
    std::ofstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    file << "digraph {" << std::endl;
    file << "\trankdir=LR;" << std::endl; // Setting direction to Left-to-Right

    for (auto& fromState : table) {
        if(this->dead_states.find(fromState.first) != this->dead_states.end()) continue;
        for (auto& transition : fromState.second) {
            for (auto& toState : transition.second) {
                if(this->dead_states.find(toState) != this->dead_states.end()) continue;
                file << "\t" << fromState.first->getStateId() << " -> " << toState->getStateId() << " [label=\"" << transition.first << "\"];" << std::endl;
            }
        }
    }
    // Mark specific nodes as end states and set their colors
    file << "\t";
    for (auto& state : this->end_states) {
        file << state->getStateId() << "[peripheries=2 style=filled fillcolor=yellow color=black]; ";
    }
    file << this->startState->getStateId() << " [arrowhead=normal style=filled fillcolor=green color=black];";
    file << std::endl;
    file << "}" << std::endl;
    file.close();
}

void TransitionDiagram::toCSVFile(std::string file_name) {
    std::ofstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Header row: inputs
    file << ",";
    for (const auto& input : table.begin()->second) {
        file << "\"" << input.first << "\",";
    }
    file << std::endl;

    // Data rows: states and transitions
    for (auto& stateEntry : table) {
        if(this->dead_states.find(stateEntry.first) != this->dead_states.end()) continue;
        file << stateEntry.first->getStateId() << ",";
        for (auto& input : stateEntry.second) {
            auto& nextStates = input.second;
            for (const auto& nextState : nextStates) {
                if(this->dead_states.find(nextState) != this->dead_states.end()) continue;
                file << nextState->getStateId();
            }
            file << ",";
        }
        file << std::endl;
    }

    file.close();
}

void TransitionDiagram::toMDFile(std::string file_name) {
    std::ofstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    file << "| States |";
    for (const auto& input : table.begin()->second) {
        file << " " << input.first << " |";
    }
    file << std::endl;

    file << "| --- |";
    for (size_t i = 0; i < table.begin()->second.size(); ++i) {
        file << " --- |";
    }
    file << std::endl;

    for (const auto& stateEntry : table) {
        if(this->dead_states.find(stateEntry.first) != this->dead_states.end()) continue;
        file << "| " << stateEntry.first->getStateId() << " |";
        for (const auto& input : stateEntry.second) {
            const auto& nextStates = input.second;
            for (const auto& nextState : nextStates) {
                if(this->dead_states.find(nextState) != this->dead_states.end()) continue;
                file << nextState->getStateId();
            }
            file << "|";
        }
        file << std::endl;
    }

    file.close();
}

void TransitionDiagram::fillTable(const NFAState* state, std::vector<const NFAState*> end_states, std::vector<std::string> tokens, std::unordered_map<const NFAState*, std::string> states_tokens_map, bool new_fill) {
    this->startState = state;
    this->tokens = tokens;
    this->end_states = std::unordered_set<const NFAState*>(end_states.begin(), end_states.end());
    std::queue<const NFAState*> queue;
    std::unordered_set<const NFAState*> visited;
    std::unordered_set<char> inputs_set;
    queue.push(state);
    while(!queue.empty()) {
        const NFAState* current_state = queue.front();
        this->state_id_map[current_state->getStateId()] = current_state;
        queue.pop();
        visited.insert(current_state);
        this->states.insert(current_state);
        std::unordered_map<char, std::vector<NFAState*>> transitions = current_state->getTransitions();
        std::unordered_map<char, std::vector<const NFAState*>> table_entry;
        for(auto &kv : transitions) {
            char input = kv.first;
            inputs_set.insert(input);
            if(table_entry.find(input) == table_entry.end()) {
                table_entry[input] = std::vector<const NFAState*>();
            }
            for(auto &next_state : kv.second) {
                table_entry[input].push_back(next_state);
                if(visited.find(next_state) == visited.end()) {
                    queue.push(next_state);
                }
            }
        }
        this->table[current_state] = table_entry; 
        if(TransitionDiagram::isDeadState(current_state, this->end_states)) {
            this->dead_states.insert(current_state);
        }
    }
    for(auto c : inputs_set) {
        this->inputs.push_back(c);
    }
    if(new_fill) {
        for(auto st : visited) {
            if(st->isEndState()) {
                this->end_states_tokens_map[st] = st->getTokenName(); 
            }
        }
    }
    else {
        this->end_states_tokens_map = states_tokens_map;
    }
}

const NFAState* TransitionDiagram::getStateId(int state_id) {
    if(this->state_id_map.find(state_id) == state_id_map.end()) return nullptr;
    const NFAState* state = this->state_id_map[state_id];
    return state;
}

/**
 * @warning new objects created here
 * @todo figure out a way to prevent potential memory leaks 
 */
const NFAState* TransitionDiagram::mergeStates(TransitionDiagram* transdig,std::map<std::set<const NFAState*>, std::map<char, std::set<const NFAState*>>>& new_table, std::vector<const NFAState*>& new_end_states, std::unordered_map<const NFAState*, std::string>& new_end_states_tokens_map) {
    std::map<std::set<const NFAState*>, NFAState*> merge_map;
    const NFAState* new_start_state = nullptr;
    for(auto kv : new_table) {
        // Newly Created states should be deleted using the NFAState Destructor 
        NFAState* new_state = new NFAState();
        if(TransitionDiagram::isEndStateNew(kv.first, transdig->getEndStates())) {
            std::vector<std::string> tokens;
            for(auto kv2 : kv.first) {
                if(transdig->getEndStatesTokensMap().find(kv2) != transdig->getEndStatesTokensMap().end()) {
                    tokens.push_back(transdig->getEndStatesTokensMap()[kv2]);
                }
            }
            new_end_states.push_back(new_state);
            new_end_states_tokens_map[new_state] = transdig->getMaxPriorityToken(tokens);
        }
        merge_map[kv.first] = new_state;
        for(auto state : kv.first) {
            if(transdig->getStartState() == state) {
                new_start_state = merge_map[kv.first];
            }
        }
    }
    NFAState* dead_state = nullptr;
    for(auto kv : new_table) {
        NFAState* current_state = merge_map[kv.first];
        for(auto kv2 : kv.second) {
            if(merge_map.find(kv2.second) != merge_map.end()) {
                current_state->addTransition(kv2.first, merge_map[kv2.second]);
            }
            else {
                if(dead_state == nullptr) {
                    dead_state = new NFAState();
                    for(auto c : transdig->getInputs()) {
                        if(c != EPSILON) {
                            dead_state->addTransition(c, dead_state);
                        }
                    }
                }
                current_state->addTransition(kv2.first, dead_state);
            }
        }
    }
    return new_start_state;
}

bool TransitionDiagram::isDeadState(const NFAState* state, std::unordered_set<const NFAState*> end_states) {
    bool dead = true;
    std::unordered_map<char,  std::vector<NFAState*>> trans = state->getTransitions();
    if(trans.size() <= 0) return false;
    for(auto kv : trans) {
        if(kv.second.size() <= 0) return false;
        for(auto next_state : kv.second) {
            if(next_state != state) {
                dead = false;
                break;
            }
        }
        if(!dead) break;
    }
    if(end_states.find(state) != end_states.end()) return false;
    return dead;
}

bool TransitionDiagram::isEndStateNew(std::set<const NFAState*> states, std::unordered_set<const NFAState*> end_states) {
    for(auto state : states) {
        if(end_states.find(state) != end_states.end()) return true;
    }
    return false;
}

std::string TransitionDiagram::getMaxPriorityToken(std::vector<std::string> tokens) {
    int min_prio = 100000;
    std::string min_token;
    for(auto tk : tokens) {
        int prio = this->tokensPriority[tk];
        if(prio < min_prio) {
            min_prio = prio;
            min_token = tk;
        }
    }
    return min_token;
}
