#include <gtest/gtest.h>
#include "NFA.h"

class NFATest : public ::testing::Test {
protected:
    void SetUp() override {
        nfa = new NFA();
        nfaState1 = new NFAState();
        nfaState2 = new NFAState();
        nfaState3 = new NFAState();
        nfa->addEndState(nfaState2);
        nfa->addEndState(nfaState3);

        nfa->getStartState()->addTransition('a', nfaState1);
        nfa->getStartState()->addTransition('b', nfaState2);
        nfa->getStartState()->addTransition('c', nfaState2);
        nfaState1->addTransition('d', nfaState2);
        nfaState1->addTransition('e', nfaState3);
    }

    void TearDown() override {
    }

    NFA *nfa{};
    NFAState *nfaState1{};
    NFAState *nfaState2{};
    NFAState *nfaState3{};
};

TEST_F(NFATest, Constructor_DefaultConstructor) {
    // Check if start and end states are initialized correctly
    ASSERT_NE(nfa->getStartState(), nullptr);
    ASSERT_NE(nfa->getEndState(), nullptr);
    // Ensure start state is not the same as end state
    ASSERT_NE(nfa->getStartState()->getStateId(), nfa->getEndState()->getStateId());
}

TEST_F(NFATest, Constructor_NFAWithStates) {
    NFA nfaWithStates(nfaState1, nfaState2);

    NFAState *startState = nfaWithStates.getStartState();
    NFAState *endState = nfaWithStates.getEndState();

    ASSERT_EQ(&(*startState), &(*nfaState1));
    ASSERT_EQ(&(*endState), &(*nfaState2));
}

TEST_F(NFATest, CopyConstructor_ValidCopy) {
    nfa->setTokenName("TOKEN");

    NFA copiedNFA(*nfa);

    ASSERT_NE(copiedNFA.getStartState(), nfa->getStartState());
    ASSERT_NE(copiedNFA.getEndState(), nfa->getEndState());
}

TEST_F(NFATest, Destructor_NFA) {
    // Ensure no memory leaks
    ASSERT_NO_THROW(delete nfa);
}

TEST_F(NFATest, GetTokenName_ValidName) {
    nfa->setTokenName("TOKEN");

    std::string tokenName = nfa->getTokenName();

    ASSERT_EQ(tokenName, "TOKEN");
}

TEST_F(NFATest, AddEndState_ValidState) {
    nfa->addEndState(nfaState1);

    ASSERT_EQ(nfa->getEndStates().size(), 3);
}

TEST_F(NFATest, basicCharToNFA_ValidChar) {
    char c = 'a';

    NFA *nfa = NFA::basicCharToNFA(c);

    ASSERT_NE(nfa, nullptr);
    ASSERT_NE(nfa->getStartState(), nullptr);
    ASSERT_NE(nfa->getEndState(), nullptr);
    ASSERT_EQ(nfa->getStartState()->getTransitions().size(), 1);
    ASSERT_EQ(nfa->getStartState()->getTransitions()['a'].size(), 1);
    ASSERT_EQ(nfa->getStartState()->getTransitions()['a'][0], nfa->getEndState());
}

TEST_F(NFATest, wordToNFA_ValidWord) {
    std::string word = "hello";

    NFA *nfa = NFA::wordToNFA(word);

    ASSERT_NE(nfa, nullptr);
    ASSERT_NE(nfa->getStartState(), nullptr);
    ASSERT_NE(nfa->getEndState(), nullptr);
    ASSERT_EQ(nfa->getStartState()->getTransitions().size(), 1);
    ASSERT_EQ(nfa->getStartState()->getTransitions()['h'].size(), 1);
    ASSERT_NE(nfa->getStartState()->getTransitions()['h'][0], nfa->getEndState());
}

TEST_F(NFATest, unionNAFs_ValidNFAs) {
    NFA *nfa1 = new NFA();
    NFA *nfa2 = new NFA();

    NFA *unionNFA = NFA::unionNAFs(nfa1, nfa2);

    ASSERT_NE(unionNFA, nullptr);
    ASSERT_NE(unionNFA->getStartState(), nullptr);
    ASSERT_NE(unionNFA->getEndState(), nullptr);
    ASSERT_EQ(nfa1->getEndState()->getTransitions()['#'].size(), 1);
    ASSERT_EQ(nfa2->getEndState()->getTransitions()['#'].size(), 1);
    ASSERT_EQ(nfa1->getEndState()->getTransitions()['#'][0], unionNFA->getEndState());
    ASSERT_EQ(nfa2->getEndState()->getTransitions()['#'][0], unionNFA->getEndState());
}

TEST_F(NFATest, concatNAFs_ValidNFAs) {
    NFA *nfa1 = new NFA();
    NFA *nfa2 = new NFA();

    NFA *concatNFA = NFA::concatNAFs(nfa1, nfa2);

    ASSERT_NE(concatNFA, nullptr);
    ASSERT_NE(concatNFA->getStartState(), nullptr);
    ASSERT_NE(concatNFA->getEndState(), nullptr);
    ASSERT_EQ(nfa1->getStartState(), concatNFA->getStartState());
    ASSERT_EQ(nfa2->getEndState(), concatNFA->getEndState());
}

TEST_F(NFATest, kleeneStarNFA_ValidNFA) {
    NFA *kleeneStarNFA = NFA::kleeneStarNFA(nfa);

    ASSERT_NE(kleeneStarNFA, nullptr);
    ASSERT_NE(kleeneStarNFA->getStartState(), nullptr);
    ASSERT_NE(kleeneStarNFA->getEndState(), nullptr);

    ASSERT_EQ(kleeneStarNFA->getStartState()->getTransitions()['#'].size(), 2);
    ASSERT_EQ(kleeneStarNFA->getStartState()->getTransitions()['#'][0], nfa->getStartState());
    ASSERT_EQ(kleeneStarNFA->getStartState()->getTransitions()['#'][1], kleeneStarNFA->getEndState());

    ASSERT_EQ(nfa->getEndState()->getTransitions()['#'].size(), 2);
    ASSERT_EQ(nfa->getEndState()->getTransitions()['#'][0], nfa->getStartState());
    ASSERT_EQ(nfa->getEndState()->getTransitions()['#'][1], kleeneStarNFA->getEndState());
}

TEST_F(NFATest, positiveClosureNFA_ValidNFA) {
    NFA *positiveClosureNFA = NFA::positiveClosureNFA(nfa);

    ASSERT_NE(positiveClosureNFA, nullptr);
    ASSERT_NE(positiveClosureNFA->getStartState(), nullptr);
    ASSERT_NE(positiveClosureNFA->getEndState(), nullptr);

    ASSERT_EQ(positiveClosureNFA->getStartState()->getTransitions()['#'].size(), 1);
    ASSERT_EQ(positiveClosureNFA->getStartState()->getTransitions()['#'][0], nfa->getStartState());

    ASSERT_EQ(nfa->getEndState()->getTransitions()['#'].size(), 2);
    ASSERT_EQ(nfa->getEndState()->getTransitions()['#'][0], nfa->getStartState());
    ASSERT_EQ(nfa->getEndState()->getTransitions()['#'][1], positiveClosureNFA->getEndState());
}

TEST_F(NFATest, unionRangeNFAs_ValidRange) {
    NFA *rangeStartNFA = new NFA();
    NFA *rangeEndNFA = new NFA();

    rangeStartNFA->getStartState()->addTransition('a', rangeStartNFA->getEndState());
    rangeEndNFA->getStartState()->addTransition('z', rangeEndNFA->getEndState());

    NFA *unionRangeNFA = NFA::unionRangeNFAs(rangeStartNFA, rangeEndNFA);

    ASSERT_NE(unionRangeNFA, nullptr);
    ASSERT_NE(unionRangeNFA->getStartState(), nullptr);
    ASSERT_NE(unionRangeNFA->getEndState(), nullptr);

    auto startTransitions = unionRangeNFA->getStartState()->getTransitions()['#'];
    ASSERT_EQ(startTransitions.size(), 26);
}