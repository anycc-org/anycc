#include <gtest/gtest.h>
#include "DeterministicTransitionDiagramCreator.h"
#include "NFAState.h"

class DeterministicTransitionDiagramCreatorTest : public ::testing::Test {
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


TEST_F(DeterministicTransitionDiagramCreatorTest, SubsetConstruction_validInput_returnsModifiedDiagram_1_inplace_true) {
    state0->addTransition('0', state1);
    state0->addTransition('0', state2);
    state1->addTransition('1', state2);
    state1->addTransition('1', state0);
    state2->addTransition('1', state3);
    state2->addTransition('0', state3);
    state3->addTransition('1', state3);
    state3->addTransition('0', state1);

    state1->setTokenName("token1");
    state2->setTokenName("token2");

    // Create a TransitionDiagram
    TransitionDiagram diagram(state0, {state1, state2}, {"token1", "token2"},
                              {{"token1", 1}, {"token2", 2}});

    // Test the filled table
    ASSERT_EQ(diagram.getStates().size(), 4); // Three states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 2); // Two end states should be present
    ASSERT_EQ(diagram.getTokens().size(), 2); // Two tokens should be present
    ASSERT_TRUE(!diagram.lookup(state2, '1').empty()); // Check if transitions are correctly filled

    // Call the subsetConstruction function
    DeterministicTransitionDiagramCreator::subsetConstruction(&diagram);

    // Test the filled table
    ASSERT_EQ(diagram.getStates().size(), 9); // Three states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 6); // Two end states should be present
}


TEST_F(DeterministicTransitionDiagramCreatorTest, SubsetConstruction_validInput_returnsModifiedDiagram_2_inplace_true) {
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
    ASSERT_EQ(diagram.getStates().size(), 11); // Three states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 1); // Two end states should be present
    ASSERT_EQ(diagram.getTokens().size(), 1); // Two tokens should be present
    ASSERT_TRUE(!diagram.lookup(state2, 'a').empty()); // Check if transitions are correctly filled

    // Call the subsetConstruction function
    DeterministicTransitionDiagramCreator::subsetConstruction(&diagram);

    // Test the filled table
    ASSERT_EQ(diagram.getStates().size(), 5); // Three states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 1); // Two end states should be present
}


TEST_F(DeterministicTransitionDiagramCreatorTest,
       SubsetConstruction_validInput_returnsModifiedDiagram_2_inplace_false) {
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
    ASSERT_EQ(diagram.getStates().size(), 11); // Three states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 1); // Two end states should be present
    ASSERT_EQ(diagram.getTokens().size(), 1); // Two tokens should be present
    ASSERT_TRUE(!diagram.lookup(state2, 'a').empty()); // Check if transitions are correctly filled

    // Call the subsetConstruction function
    auto new_diagram = DeterministicTransitionDiagramCreator::subsetConstruction(&diagram, false);

    // Test the filled table
    ASSERT_EQ(new_diagram->getStates().size(), 5); // Three states should be present
    ASSERT_EQ(new_diagram->getEndStates().size(), 1); // Two end states should be present
}
