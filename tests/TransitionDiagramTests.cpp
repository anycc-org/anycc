#include <gtest/gtest.h>
#include "TransitionDiagram.h"
#include "NFAState.h"

class TransitionDiagramTest : public ::testing::Test {
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
        state11 = new NFAState();
        state12 = new NFAState();
    }

    void TearDown() override {
        // Any cleanup code needed after each test
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
    NFAState *state11{};
    NFAState *state12{};
};

TEST_F(TransitionDiagramTest, FillTable) {
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
}

TEST_F(TransitionDiagramTest, mergeStates_ValidInput_1) {
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

    // Create a map of sets of NFAState pointers
    std::map<std::set<const NFAState *>, std::map<char, std::set<const NFAState *>>> table;
    std::set<const NFAState *> stateSet1 = {state0, state1, state3, state7};
    std::set<const NFAState *> stateSet2 = {state2, state4, state7};
    std::set<const NFAState *> stateSet3 = {state7};
    std::set<const NFAState *> stateSet4 = {state8};
    std::set<const NFAState *> stateSet5 = {state8, state6};
    std::set<const NFAState *> stateSet6 = {state5, state8};

    table[stateSet1] = {{'a', stateSet2},
                        {'b', stateSet4}};
    table[stateSet2] = {{'a', stateSet3},
                        {'b', stateSet6}};
    table[stateSet3] = {{'a', stateSet3},
                        {'b', stateSet4}};
    table[stateSet4] = {{'b', stateSet4}};
    table[stateSet5] = {{'b', stateSet4}};
    table[stateSet6] = {{'b', stateSet5}};

    // Create vectors for end states and tokens
    std::vector<const NFAState *> new_end_states;
    std::unordered_map<const NFAState *, std::string> new_end_states_tokens_map;

    // Call mergeStates
    const NFAState *new_start_state = TransitionDiagram::mergeStates(&diagram, table, new_end_states,
                                                                     new_end_states_tokens_map);
    diagram.clear();
    diagram.fillTable(new_start_state, new_end_states, diagram.getTokens(),
                      new_end_states_tokens_map, false);

    // Test the merged states
    ASSERT_EQ(diagram.getStates().size(), 6); // 6 states should be present after merging
    ASSERT_EQ(diagram.getEndStates().size(), 4); // 4 end states should be present after merging
    ASSERT_TRUE(!diagram.lookup(new_start_state, 'a').empty()); // Check if transitions are correctly merged
}

TEST_F(TransitionDiagramTest, mergeStates_ValidInput_2) {
    // NFA for regular expressions do, double, id
    state0->addTransition('#', state1);
    state0->addTransition('#', state4);
    state0->addTransition('#', state11);
    state1->addTransition('d', state2);
    state2->addTransition('o', state3);
    state4->addTransition('d', state5);
    state5->addTransition('o', state6);
    state6->addTransition('u', state7);
    state7->addTransition('b', state8);
    state8->addTransition('l', state9);
    state9->addTransition('e', state10);

    for (char c = 'a'; c <= 'z'; c++) {
        state11->addTransition(c, state12);
    }

    state3->setTokenName("do");
    state10->setTokenName("double");
    state12->setTokenName("id");

    // Create another TransitionDiagram for merging
    TransitionDiagram diagram(state0, {state3, state10, state12}, {"do", "double", "id"},
                              {{"do",     0},
                               {"double", 0},
                               {"id",     3}});

    ASSERT_EQ(diagram.getStates().size(), 13); // 13 states should be present
    ASSERT_EQ(diagram.getEndStates().size(), 3); // 3 end states should be present
    ASSERT_EQ(diagram.getTokens().size(), 3); // 3 tokens should be present
    ASSERT_TRUE(!diagram.lookup(state11, 'b').empty()); // Check if transitions are correctly filled

    // Create a map of sets of NFAState pointers
    std::map<std::set<const NFAState *>, std::map<char, std::set<const NFAState *>>> table;
    std::set<const NFAState *> stateSet1 = {state0, state1, state4, state11};
    std::set<const NFAState *> stateSet2 = {state2, state5, state12};
    std::set<const NFAState *> stateSet3 = {state3, state6};
    std::set<const NFAState *> stateSet4 = {state7};
    std::set<const NFAState *> stateSet5 = {state8};
    std::set<const NFAState *> stateSet6 = {state9};
    std::set<const NFAState *> stateSet7 = {state10};
    std::set<const NFAState *> stateSet8 = {state12};

    for (char c = 'a'; c <= 'z'; c++) {
        if (c != 'd')
            table[stateSet1][c] = stateSet8;
        else
            table[stateSet1][c] = stateSet2;
    }
    table[stateSet2] = {{'o', stateSet3}};
    table[stateSet3] = {{'u', stateSet4}};
    table[stateSet4] = {{'b', stateSet5}};
    table[stateSet5] = {{'l', stateSet6}};
    table[stateSet6] = {{'e', stateSet7}};
    for (char c = 'a'; c <= 'z'; c++) {
        table[stateSet7][c] = stateSet8;
    }

    // Create vectors for end states and tokens
    std::vector<const NFAState *> new_end_states;
    std::unordered_map<const NFAState *, std::string> new_end_states_tokens_map;

    // Call mergeStates
    const NFAState *new_start_state = TransitionDiagram::mergeStates(&diagram, table, new_end_states,
                                                                     new_end_states_tokens_map);
    diagram.clear();
    diagram.fillTable(new_start_state, new_end_states, diagram.getTokens(),
                      new_end_states_tokens_map, false);

    // Test the merged states
    ASSERT_EQ(diagram.getStates().size(), 8); // 8 states should be present after merging
    ASSERT_EQ(diagram.getEndStates().size(), 3); // 3 end states should be present after merging
    ASSERT_EQ(diagram.getDeadStates().size(), 1);
    ASSERT_TRUE(!diagram.lookup(new_start_state, 'a').empty()); // Check if transitions are correctly merged
}

TEST_F(TransitionDiagramTest, getRecursiveEpsilonClosure_validInput) {
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

    auto visited0 = diagram.getRecursiveEpsilonClosure(state0);
    std::set<const NFAState *> expected_states = {state0, state1, state2, state4, state7};
    ASSERT_EQ(visited0.size(), 5);
    for (auto state: visited0) {
        ASSERT_TRUE(expected_states.find(state) != expected_states.end());
    }
}