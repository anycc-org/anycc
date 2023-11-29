#include <Lex/NFA.h>
#include <iostream>

NFA::NFA() {
    startState = new NFAState();
    endState = new NFAState();
}

NFA::~NFA() = default;

NFAState* NFA::getStartState() const { return startState; }

//void NFA::setStartState(const NFAState &state) { startState = state; }

NFAState* NFA::getEndState() const { return endState; }

//void NFA::setEndState(const NFAState &state) { endState = state; }

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
    nfa1->endState->addTransition('e', nfa2->startState);
    nfa1->endState = nfa2->endState;
    return nfa1;
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

NFA* NFA::unionRangeNFAs(NFA* rangeStartNFA, NFA* rangeEndNFA) {
//    auto startTrans = rangeStartNFA.startState.getTransitions();
//    auto endTrans = rangeEndNFA.startState.getTransitions();
//
//    if (startTrans.count('0') && endTrans.count('9')) { // 0 - 9 encountered
//        return
//    }
//    else if (startTrans.count('a') && endTrans.count('z')) { // a - z encountered
//
//    }
//    else if (startTrans.count('A') && endTrans.count('Z')) { // A - Z encountered
//
//    }
//    else {
//        // Throw an Error
//    }

}

NFA NFA::constructDigitNFA(NFA &startDigitNFA) {
//    std::vector<NFA> digits(10);
//    digits.push_back(startDigitNFA);
//    for (int i = 1; i <= 9; i++) { // 1 '1'
//        digits.push_back(NFA::basicCharToNFA('0' + i));
//    }
//    NFA digitNFA;
//    for (int i = 0; i < 10; i++) {
//        digitNFA.startState->addTransition('e', digits[i].startState);
//        digits[i].endState->addTransition('e', digitNFA.endState);
//    }
//    return digitNFA;
}

NFA NFA::constructLowerCaseNFA(NFA &startLetterNFA) {}

NFA NFA::constructUpperCaseNFA(NFA &startLetterNFA) {}

/**
 * Prints the NFA.
 */
void NFA::printNFA() const {
    std::cout << "Start state: " << startState->getStateId() << std::endl;
    std::cout << "End state: " << endState->getStateId() << std::endl;
    startState->printState();
}