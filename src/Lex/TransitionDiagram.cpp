#include "Lex/NFA.h"
#include "Lex/NFAState.h"
#include <Lex/TransitionDiagram.h>
#include <map>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>


TransitionDiagram::TransitionDiagram(const NFAState* start_state, std::vector<const NFAState*> end_states) {
    this->fillTable(start_state, end_states);
}

std::vector<char> TransitionDiagram::getInputs() const {
    return std::vector<char>(this->inputs.begin(), this->inputs.end());
}

std::vector<const NFAState*> TransitionDiagram::getStates() const {
    return std::vector<const NFAState*>(this->states.begin(), this->states.end());
}

const NFAState* TransitionDiagram::getStartState() const {
    return this->startState;
}

std::vector<const NFAState*> TransitionDiagram::getEndStates() const {
    return std::vector<const NFAState*>(this->end_states.begin(), this->end_states.end());
}

std::vector<const NFAState*> TransitionDiagram::getDeadStates() const {
    return std::vector<const NFAState*>(this->dead_states.begin(), this->dead_states.end());
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


std::vector<const NFAState*> TransitionDiagram::getRecursiveEpsilonClosure(const NFAState* state) {
    std::stack<const NFAState*> stack;
    std::unordered_set<const NFAState*> visited;
    stack.push(state);
    while(!stack.empty()) {
        const NFAState* current_state = stack.top();
        stack.pop();
        visited.insert(current_state);
        std::vector<const NFAState*> next_states = this->lookup(current_state, 'e');
        for(auto next_state : next_states) {
            if(visited.find(next_state) == visited.end()) {
                stack.push(next_state);
            }
        }
    }
    return std::vector<const NFAState*>(visited.begin(), visited.end());
}

std::vector<const NFAState*> TransitionDiagram::getAllNextStates(std::vector<const NFAState*>& states, char input) {
    std::vector<const NFAState*> next_states;
    for(auto state : states) {
        std::vector<const NFAState*> next_stats = this->lookup(state, input);
        for(auto new_next_state : next_stats) {
            next_states.push_back(new_next_state);
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
    return removeEpsilonTransitionsInplace(new TransitionDiagram(this->getStartState(), this->getEndStates()));
}

TransitionDiagram* TransitionDiagram::subsetConstruction(bool inplace) {
    if(inplace) return subsetConstructionInplace(this);
    return subsetConstructionInplace(new TransitionDiagram(this->getStartState(), this->getEndStates()));
}

TransitionDiagram* TransitionDiagram::minimize(bool inplace) {
    if(inplace) return minimizeInplace(this);
    return minimizeInplace(new TransitionDiagram(this->getStartState(), this->getEndStates()));
}

TransitionDiagram* TransitionDiagram::removeEpsilonTransitionsInplace(TransitionDiagram* transdig) {
    std::vector<char> inputs = transdig->getInputs();
    std::vector<const NFAState*> states = transdig->getStates();
    for(auto state : states) {
        std::vector<const NFAState*> closure_states = transdig->getRecursiveEpsilonClosure(state);
        for(auto c : inputs) {
            std::unordered_set<const NFAState*> result_states;
            if(c != 'e') {
                std::vector<const NFAState*> next_states = transdig->getAllNextStates(closure_states, c);
                for(auto tmp_state : next_states) {
                    std::vector<const NFAState*> closure_next_state = transdig->getRecursiveEpsilonClosure(tmp_state);
                    for(auto next_state : closure_next_state) {
                        result_states.insert(next_state);
                    }
                }
                transdig->table[state][c] = std::vector<const NFAState*>(result_states.begin(), result_states.end());
            }
        }
    }
    for(auto state : states) {
        transdig->table[state].erase('e');
    }
    transdig->inputs.erase('e');
    return transdig;
}

TransitionDiagram* TransitionDiagram::subsetConstructionInplace(TransitionDiagram* transdig) {
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
        for(auto c : transdig->inputs) {
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
    const NFAState* new_start_state = TransitionDiagram::mergeStates(new_table, start_state, new_end_states, transdig->getInputs());
    transdig->inputs.clear();
    transdig->states.clear();
    transdig->table.clear();
    transdig->state_id_map.clear();
    transdig->end_states.clear();
    transdig->dead_states.clear();
    transdig->fillTable(new_start_state, new_end_states);
    return transdig;
}

TransitionDiagram* TransitionDiagram::minimizeInplace(TransitionDiagram* transdig) {
    std::vector<std::vector<std::unordered_set<const NFAState*>>> all_sets;
    all_sets.push_back(std::vector<std::unordered_set<const NFAState*>>());
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

void TransitionDiagram::fillTable(const NFAState* state, std::vector<const NFAState*> end_states) {
    this->startState = state;
    this->end_states = std::unordered_set<const NFAState*>(end_states.begin(), end_states.end());
    std::queue<const NFAState*> queue;
    std::unordered_set<const NFAState*> visited;
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
            this->inputs.insert(input);
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
        if(TransitionDiagram::isDeadState(current_state)) {
            this->dead_states.insert(current_state);
        }
    }
}

const NFAState* TransitionDiagram::getStateId(int state_id) {
    if(this->state_id_map.find(state_id) == state_id_map.end()) return nullptr;
    const NFAState* state = this->state_id_map[state_id];
    return state;
}

const NFAState* TransitionDiagram::mergeStates(std::map<std::vector<const NFAState*>,  std::map<char, std::vector<const NFAState*>>>& new_table, const NFAState* start_state,  std::vector<const NFAState*>& new_end_states, std::vector<char> inputs) {
    std::map<std::vector<const NFAState*>, NFAState*> merge_map;
    const NFAState* new_start_state = nullptr;
    for(auto kv : new_table) {
        NFAState* new_state = new NFAState();
        if(TransitionDiagram::isDeadStateNew(kv.first)) {
            new_end_states.push_back(new_state);
        }
        merge_map[kv.first] = new_state;
        for(auto state : kv.first) {
            if(start_state == state) {
                new_start_state = merge_map[kv.first];
            }
        }
    }
    for(auto kv : new_table) {
        NFAState* current_state = merge_map[kv.first];
        for(auto kv2 : kv.second) {
            if(merge_map.find(kv2.second) != merge_map.end()) {
                current_state->addTransition(kv2.first, merge_map[kv2.second]);
            }
            else {
                NFAState* dead_state = new NFAState();
                current_state->addTransition(kv2.first, dead_state);
                for(auto c : inputs) {
                    if(c != 'e') {
                        dead_state->addTransition(c, dead_state);
                    }
                }
            }
        }
    }
    return new_start_state;
}

bool TransitionDiagram::isDeadState(const NFAState* state) {
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
    return dead;
}

bool TransitionDiagram::isDeadStateNew(std::vector<const NFAState*> states) {
    for(auto state : states) {
        if(state->isEndState()) return true;
    }
    return false;
}