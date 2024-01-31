#include <gtest/gtest.h>
#include "NFAState.h"

class NFAStateTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Teardown code if needed
    }
};

TEST_F(NFAStateTest, addTransition_ValidTransition_TransitionAdded) {
    // Arrange
    NFAState state1;
    NFAState state2;

    // Act
    state1.addTransition('a', &state2);

    // Assert
    auto transitions = state1.getTransitions();
    ASSERT_EQ(transitions.size(), 1);
    ASSERT_EQ(transitions['a'].size(), 1);
    ASSERT_EQ(transitions['a'][0], &state2);
}

TEST_F(NFAStateTest, setTokenName_ValidTokenName_TokenNameSet) {
    // Arrange
    NFAState state;

    // Act
    state.setTokenName("TOKEN");

    // Assert
    ASSERT_EQ(state.getTokenName(), "TOKEN");
}

TEST_F(NFAStateTest, isEndState_NoTransitions_ReturnsTrue) {
    // Arrange
    NFAState state;

    // Act & Assert
    ASSERT_TRUE(state.isEndState());
}

TEST_F(NFAStateTest, isEndState_WithTransitions_ReturnsFalse) {
    // Arrange
    NFAState state1;
    NFAState state2;
    state1.addTransition('a', &state2);

    // Act & Assert
    ASSERT_FALSE(state1.isEndState());
}

TEST_F(NFAStateTest, copyConstructor_ValidState_StateCopiedCorrectly) {
    NFAState state1;
    NFAState state2;
    state1.addTransition('a', &state2);
    state1.setTokenName("TOKEN");

    std::unordered_map<NFAState *, NFAState *> copiedStates;
    NFAState copiedState(state1, copiedStates);

    ASSERT_NE(copiedState.getStateId(), state1.getStateId());
    ASSERT_EQ(copiedState.getTokenName(), state1.getTokenName());

    auto transitions1 = state1.getTransitions();
    auto transitions2 = copiedState.getTransitions();
    ASSERT_EQ(transitions1.size(), transitions2.size());

    for (const auto &entry: transitions1) {
        char symbol = entry.first;
        const std::vector<NFAState *> &states1 = entry.second;
        const std::vector<NFAState *> &states2 = transitions2[symbol];
        ASSERT_EQ(states1.size(), states2.size());
        for (size_t i = 0; i < states1.size(); ++i) {
            ASSERT_NE(states1[i], states2[i]);  // Ensure original pointers are not used
            ASSERT_NE(states1[i]->getStateId(), states2[i]->getStateId());
        }
    }
}