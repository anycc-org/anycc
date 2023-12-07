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
    std::unordered_map<int, NFAState*> copiedStates;
    endState = new NFAState(*(other.endState), copiedStates);
    endState->setTokenName(other.endState->getTokenName());
    startState = new NFAState(*(other.startState), copiedStates);
    copiedStates.clear();
}

NFA::~NFA() { delete startState; }

NFAState* NFA::getStartState() const { return startState; }

NFAState* NFA::getEndState() const { return endState; }

std::string NFA::getTokenName() const { return endState->getTokenName(); }

void NFA::setTokenName(const std::string& name) { endState->setTokenName(name); }

void NFA::addEndState(NFAState *state) { endStates.push_back(state); }

/**
 * @brief it creates a basic NFA with two states and a transition labeled with
 * the input character c from the start state to the end state. eg. s -- c --> f
 * @param c a character from the input alphabet.
 * @return basic NFA of c.
 */
NFA* NFA::basicCharToNFA(char c) {
    NFA *nfa = new NFA();
    nfa->startState->addTransition(c, nfa->endState);
    return nfa;
}

/**
 * @brief it creates a chain of states, each representing a character in the word,
 * with transitions labeled with the characters of the word.
 * @return NFA of word
 */
NFA* NFA::wordToNFA(const std::string& word) {
    if (word.size() == 1) {
        return basicCharToNFA(word[0]);
    }
    NFA* wordNFA = new NFA();
    NFAState* currentState = wordNFA->getStartState();

    for (char c : word) {
        if (c == '\\') { // eg ab\+c -> ab+c
            continue;
        }
        NFAState* nextState = new NFAState();
        currentState->addTransition(c, nextState);
        currentState = nextState;
    }

    wordNFA->endState = currentState;
    return wordNFA;
}

/**
 * @brief It adds epsilon transitions from a new start state to the start states
 * of both input NFAs and from the end states of both input NFAs to a new end state.
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
 * @brief It connects the end state of the first NFA to the start state of the second NFA
 * @return concatenation NFA of nfa1 . nfa2.
 */
NFA* NFA::concatNAFs(NFA* nfa1, NFA* nfa2) {
    NFA* resNFA = new NFA(nfa1->startState, nfa2->endState);
    nfa1->endState->setTransitions(nfa2->startState->getTransitions());
    return resNFA;
}

/**
 * @brief It adds epsilon transitions from a new start state to the start state of nfa and
 * from the end state of nfa to both the start state and end state of the new NFA.
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
 * @brief It adds epsilon transitions from a new start state to the start state of nfa and
 * from the end state of nfa to both the start state of nfa and end state of the new NFA.
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
 * @brief It creates epsilon transitions to connect NFAs for all characters within the specified range.
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