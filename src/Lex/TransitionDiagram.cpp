#include "Lex/NFA.h"
#include "Lex/NFAState.h"
#include <Lex/TransitionDiagram.h>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>


TransitionDiagram::TransitionDiagram(const NFAState* start_state) {
    this->startState = start_state;
    this->fillTable(start_state);
}

std::vector<char> TransitionDiagram::getInputs() {
    return std::vector<char>(this->inputs.begin(), this->inputs.end());
}

std::vector<const NFAState*> TransitionDiagram::getStates() {
    return std::vector<const NFAState*>(this->states.begin(), this->states.end());
}

const NFAState* TransitionDiagram::getStartState() {
    return this->startState;
}

const NFAState* TransitionDiagram::getEndState() {
    return this->endtState;
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


std::vector<const NFAState*> TransitionDiagram::getEpsilonClosure(const NFAState* state) {
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

std::vector<const NFAState*> TransitionDiagram::getAllNextStates(std::vector<const NFAState*> states, char input) {
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
    return removeEpsilonTransitionsInplace(new TransitionDiagram(this->getStartState()));
}


TransitionDiagram* TransitionDiagram::subsetConstruction(bool inplace) {
    if(inplace) return subsetConstructionInplace(this);
    return subsetConstructionInplace(new TransitionDiagram(this->getStartState()));
}

TransitionDiagram* TransitionDiagram::removeEpsilonTransitionsInplace(TransitionDiagram* transdig) {
    std::vector<char> inputs = transdig->getInputs();
    std::vector<const NFAState*> states = transdig->getStates();
    for(auto state : states) {
        std::vector<const NFAState*> closure_states = transdig->getEpsilonClosure(state);
        for(auto c : inputs) {
            std::unordered_set<const NFAState*> result_states;
            if(c != 'e') {
                std::vector<const NFAState*> next_states = transdig->getAllNextStates(closure_states, c);
                for(auto tmp_state : next_states) {
                    std::vector<const NFAState*> closure_next_state = transdig->getEpsilonClosure(tmp_state);
                    for(auto next_state : closure_next_state) {
                        result_states.insert(next_state);
                    }
                }
                this->table[state][c] = std::vector<const NFAState*>(result_states.begin(), result_states.end());
            }
        }
    }
    for(auto state : states) {
        this->table[state].erase('e');
    }
    return transdig;
}

TransitionDiagram* TransitionDiagram::subsetConstructionInplace(TransitionDiagram* transdig) {

}


void TransitionDiagram::print() const {
    for(auto kv : this->table) {
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



void TransitionDiagram::fillTable(const NFAState* state) {
    std::queue<const NFAState*> queue;
    std::unordered_set<const NFAState*> visited;
    queue.push(state);
    while(!queue.empty()) {
        const NFAState* current_state = queue.front();
        if(current_state->isEndState()) this->endtState = current_state;
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
    }
}

const NFAState* TransitionDiagram::getStateId(int state_id) {
    if(this->state_id_map.find(state_id) == state_id_map.end()) return nullptr;
    const NFAState* state = this->state_id_map[state_id];
    return state;
}






