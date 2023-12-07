#include <Lex/NFA.h>
#include <iostream>
#include <stack>

NFA::NFA() {
    startState = new NFAState();
    endState = new NFAState();
}

NFA::NFA(NFAState *start, NFAState *end) {
    startState = start;
    endState = end;
}

NFA::NFA(const NFA& other) noexcept { // copy constructor
    std::unordered_map<NFAState*, NFAState*> copiedStates;
    startState = new NFAState(*(other.startState), copiedStates);
    endState = copiedStates[other.endState];
    for (auto& state: other.endStates) {
        endStates.push_back(copiedStates[state]);
    }
    copiedStates.clear();
}

NFA::~NFA() { delete startState; }

NFAState* NFA::getStartState() const { return startState; }

NFAState* NFA::getEndState() const { return endState; }

std::string NFA::getTokenName() const { return endState->getTokenName(); }

void NFA::setTokenName(const std::string& name) { endState->setTokenName(name); }

void NFA::addEndState(NFAState *state) { endStates.push_back(state); }

/**
 * A symbol a of the input alphabet is converted to an NFA with two states.
 *
 * @param c input symbol.
 * @return basic NFA q -- c --> f.
 */
NFA* NFA::basicCharToNFA(char c) {
    NFA *nfa = new NFA();
    nfa->startState->addTransition(c, nfa->endState);
    return nfa;
}

/**
 * @return NFA of word
 */
NFA* NFA::wordToNFA(const std::string& word) {
    if (word.size() == 1) {
        return basicCharToNFA(word[0]);
    }

    NFAState* startState = new NFAState();
    NFAState* currentState = startState;

    for (char c : word) {
        if (c == '\\') { // eg ab\+c -> ab+c
            continue;
        }
        NFAState* nextState = new NFAState();
        currentState->addTransition(c, nextState);
        currentState = nextState;
    }

    NFA* wordNFA = new NFA(startState, currentState);
    return wordNFA;
}

/**
 * @return union NFA of nfa1 | nfa2.
 */
NFA* NFA::unionNAFs(NFA *nfa1, NFA *nfa2) {
    NFA* nfa = new NFA();
    nfa->startState->addTransition('e', nfa1->startState);
    nfa->startState->addTransition('e', nfa2->startState);
    nfa1->endState->addTransition('e', nfa->endState);
    nfa2->endState->addTransition('e', nfa->endState);
    return nfa;
}

/**
 * Connect them by epsilon transition
 * @return concatenation NFA of nfa1 . nfa2.
 */
NFA* NFA::concatNAFs(NFA* nfa1, NFA* nfa2) {
    NFA* resNFA = new NFA(nfa1->startState, nfa2->endState);
    nfa1->endState->setTransitions(nfa2->startState->getTransitions());
    return resNFA;
}

/**
 * @return kleene star NFA of nfa.
 */
NFA* NFA::kleeneStarNFA(NFA* nfa) {
    NFA* nfaStar = new NFA();
    nfaStar->startState->addTransition('e', nfa->startState);
    nfaStar->startState->addTransition('e', nfaStar->endState);
    nfa->endState->addTransition('e', nfa->startState);
    nfa->endState->addTransition('e', nfaStar->endState);
    return nfaStar;
}

/**
 * @return kleene plus NFA of nfa.
 */
NFA* NFA::positiveClosureNFA(NFA* nfa) {
    NFA* nfaPlus = new NFA();
    nfaPlus->startState->addTransition('e', nfa->startState);
    nfa->endState->addTransition('e', nfa->startState);
    nfa->endState->addTransition('e', nfaPlus->endState);
    return nfaPlus;
}

/**
 * @return union NFAs on the form [a-z] or [A-Z] or [0-9]
 */
NFA* NFA::unionRangeNFAs(NFA* rangeStartNFA, NFA* rangeEndNFA) {
    NFA* rangeNFA = new NFA();

    char startSymbol = rangeStartNFA->startState->getTransitions().begin()->first;
    char endSymbol = rangeEndNFA->startState->getTransitions().begin()->first;

    rangeNFA->startState->addTransition('e', rangeStartNFA->startState);
    rangeStartNFA->endState->addTransition('e', rangeNFA->endState);

    // loop over all symbols in range
    for (char c = (char)(startSymbol + 1); c < endSymbol; c++) {
        NFA* basicNFA = NFA::basicCharToNFA(c);
        rangeNFA->startState->addTransition('e', basicNFA->startState);
        basicNFA->endState->addTransition('e', rangeNFA->endState);
    }

    rangeNFA->startState->addTransition('e', rangeEndNFA->startState);
    rangeEndNFA->endState->addTransition('e', rangeNFA->endState);

    return rangeNFA;
}

/**
 * Prints the NFA.
 */
void NFA::printNFA() const {
    std::cout << "Start state: " << startState->getStateId() << std::endl;
    std::cout << "End state: " << endState->getStateId() << "\n\n";
    startState->printState();
    std::cout << "\nTokens: ";
    for (NFAState* state : endStates) {
        std::cout << state->getTokenName() << ", ";
    }
    std::cout << std::endl;
}