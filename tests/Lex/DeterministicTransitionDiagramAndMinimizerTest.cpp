#include <gtest/gtest.h>
#include "DeterministicTransitionDiagramCreator.h"
#include "TransitionDiagramMinimizer.h"
#include "NFAState.h"

class DeterministicTransitionDiagramAndMinimizerFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Create NFAState objects
        state0 = new NFAState();
        state1 = new NFAState();
        state2 = new NFAState();
        state3 = new NFAState();
        state4 = new NFAState();
        state5 = new NFAState();
        state6 = new NFAState();
        state7 = new NFAState();
        state8 = new NFAState();
        state9 = new NFAState();
        state10 = new NFAState();
    }

    void TearDown() override {

    }

    NFAState *state0{};
    NFAState *state1{};
    NFAState *state2{};
    NFAState *state3{};
    NFAState *state4{};
    NFAState *state5{};
    NFAState *state6{};
    NFAState *state7{};
    NFAState *state8{};
    NFAState *state9{};
    NFAState *state10{};
};


TEST_F(DeterministicTransitionDiagramAndMinimizerFixture, DFA_Minimize_ValidInput_ReturnsModifiedDiagram_Inplace_True) {
    // NFA for regular expression (a|b)*abb
    state0->addTransition('#', state1);
    state0->addTransition('#', state7);
    state1->addTransition('#', state2);
    state1->addTransition('#', state4);
    state2->addTransition('a', state3);
    state3->addTransition('#', state6);
    state4->addTransition('b', state5);
    state5->addTransition('#', state6);
    state6->addTransition('#', state1);
    state6->addTransition('#', state7);
    state7->addTransition('a', state8);
    state8->addTransition('b', state9);
    state9->addTransition('b', state10);

    state10->setTokenName("(a|b)*abb");

    // Create a TransitionDiagram
    TransitionDiagram diagram(state0, {state10}, {"(a|b)*abb"}, {{"(a|b)*abb", 1}});

    // Test the filled table
    ASSERT_EQ(diagram.getStates().size(), 11); // 11 states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 1); // 1 end state should be present
    ASSERT_EQ(diagram.getTokens().size(), 1); // 1 token should be present
    ASSERT_TRUE(!diagram.lookup(state2, 'a').empty()); // Check if transitions are correctly filled

    // Call the subsetConstruction function
    DeterministicTransitionDiagramCreator::subsetConstruction(&diagram);

    // Test the filled table
    ASSERT_EQ(diagram.getStates().size(), 5); // 5 states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 1); // 1 end state should be present

    // Call the minimize function
    TransitionDiagramMinimizer::minimize(&diagram);

    // Test the filled table
    ASSERT_EQ(diagram.getStates().size(), 4); // 4 states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 1); // 1 end state should be present
}


TEST_F(DeterministicTransitionDiagramAndMinimizerFixture,
       DFA_Minimize_ValidInput_ReturnsModifiedDiagram_Inplace_False) {
    // NFA for regular expression (a|b)*abb
    state0->addTransition('#', state1);
    state0->addTransition('#', state7);
    state1->addTransition('#', state2);
    state1->addTransition('#', state4);
    state2->addTransition('a', state3);
    state3->addTransition('#', state6);
    state4->addTransition('b', state5);
    state5->addTransition('#', state6);
    state6->addTransition('#', state1);
    state6->addTransition('#', state7);
    state7->addTransition('a', state8);
    state8->addTransition('b', state9);
    state9->addTransition('b', state10);

    state10->setTokenName("(a|b)*abb");

    // Create a TransitionDiagram
    TransitionDiagram diagram(state0, {state10}, {"(a|b)*abb"}, {{"(a|b)*abb", 1}});

    // Test the filled table
    ASSERT_EQ(diagram.getStates().size(), 11); // 11 states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 1); // 1 end state should be present
    ASSERT_EQ(diagram.getTokens().size(), 1); // 1 token should be present
    ASSERT_TRUE(!diagram.lookup(state2, 'a').empty()); // Check if transitions are correctly filled

    // Call the subsetConstruction function
    auto dfa_diagram = DeterministicTransitionDiagramCreator::subsetConstruction(&diagram, false);

    // Test the filled table
    ASSERT_EQ(dfa_diagram->getStates().size(), 5); // 5 states should be present
    ASSERT_EQ(dfa_diagram->getEndStates().size(), 1); // 1 end state should be present

    // Call the minimize function
    auto min_diagram = TransitionDiagramMinimizer::minimize(dfa_diagram);

    // Test the filled table
    ASSERT_EQ(min_diagram->getStates().size(), 4); // 4 states should be present
    ASSERT_EQ(min_diagram->getEndStates().size(), 1); // 1 end state should be present
}


TEST_F(DeterministicTransitionDiagramAndMinimizerFixture,
       DFA_Minimize_ValidInput_ReturnsModifiedDiagram_MultipleEndStates) {
    // NFA for regular expressions a, abb, a*b+
    state0->addTransition('#', state1);
    state0->addTransition('#', state3);
    state0->addTransition('#', state7);
    state1->addTransition('a', state2);
    state3->addTransition('a', state4);
    state4->addTransition('b', state5);
    state5->addTransition('b', state6);
    state7->addTransition('a', state7);
    state7->addTransition('b', state8);
    state8->addTransition('b', state8);

    state2->setTokenName("a");
    state6->setTokenName("abb");
    state8->setTokenName("a*b+");

    // Create another TransitionDiagram for merging
    TransitionDiagram diagram(state0, {state2, state6, state8}, {"a", "abb", "a*b+"}, {{"a",    1},
                                                                                       {"abb",  2},
                                                                                       {"a*b+", 3}});

    ASSERT_EQ(diagram.getStates().size(), 9); // 9 states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 3); // 3 end states should be present
    ASSERT_EQ(diagram.getTokens().size(), 3); // 3 tokens should be present
    ASSERT_TRUE(!diagram.lookup(state4, 'b').empty()); // Check if transitions are correctly filled

    // Call the subsetConstruction function
    auto dfa_diagram = DeterministicTransitionDiagramCreator::subsetConstruction(&diagram);

    // Test the filled table
    ASSERT_EQ(dfa_diagram->getStates().size(), 6); // 6 states should be present
    ASSERT_EQ(dfa_diagram->getEndStates().size(), 4); // 3 end states should be present

    // Call the minimize function
    auto min_diagram = TransitionDiagramMinimizer::minimize(dfa_diagram);

    // Test the filled table
    ASSERT_EQ(min_diagram->getStates().size(), 4); // 4 states should be present
    ASSERT_EQ(min_diagram->getEndStates().size(), 2); // 3 end states should be present
}