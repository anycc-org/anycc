#include <gtest/gtest.h>
#include "Lex/NFAGenerator.h"

class NFAGeneratorFixture : public ::testing::Test {
protected:

    void SetUp() override {
    }

    void TearDown() override {
    }
};


TEST_F(NFAGeneratorFixture, BuildNFA_ValidInput_ReturnsCombinedNFA) {
    std::string num_key = "num";
    std::string id_key = "id";
    std::string relop_key = "relop";
    std::string addop_key = "addop";
    std::string mulop_key = "mulop";
    std::string assign_key = "assign";

    std::string num_value = "digit+|digit+ . digits (\\L|E digits)";
    std::string id_value = "letter (letter|digit)*";
    std::string relop_value = "\\=\\=|!\\=|>|>\\=|<|<\\=";
    std::string addop_value = "\\+|\\-";
    std::string mulop_value = "\\*|/";
    std::string assign_value = "\\=";

    std::vector<Token *> regular_expressions = {
            new Token(&num_key, &num_value),
            new Token(&id_key, &id_value),
            new Token(&relop_key, &relop_value),
            new Token(&addop_key, &addop_value),
            new Token(&mulop_key, &mulop_value),
            new Token(&assign_key, &assign_value)
    };

    std::string letter_key = "letter";
    std::string digit_key = "digit";
    std::string digits_key = "digits";

    std::string letter_value = "a-z|A-Z";
    std::string digit_value = "0-9";
    std::string digits_value = "digit+";

    std::vector<Token *> regular_definitions = {
            new Token(&letter_key, &letter_value),
            new Token(&digit_key, &digit_value),
            new Token(&digits_key, &digits_value)
    };

    std::vector<std::string> keywords = {"boolean", "int", "float", "while", "if", "else"};

    std::vector<std::string> punctuations = {";", ",", "(", ")", "{", "}"};

    // Create an NFAGenerator instance
    NFAGenerator nfaGenerator;

    // Call the buildNFA function
    NFA *combinedNFA = nfaGenerator.buildNFA(regular_expressions, regular_definitions,
                                             keywords, punctuations);

    // Assert that the combinedNFA is not null
    ASSERT_NE(combinedNFA, nullptr);

    // Assert that the start state is not null
    ASSERT_NE(combinedNFA->getStartState(), nullptr);

    // Assert that the end state is not null
    ASSERT_NE(combinedNFA->getEndState(), nullptr);

    // Assert that it has 18 end states
    ASSERT_EQ(combinedNFA->getEndStates().size(), 18);
}

TEST_F(NFAGeneratorFixture, RegexToNFA_ValidInput_ReturnsNFA) {
    std::string regex = "digit+|digit+ . digits (\\L|E digits)";

    // Create an NFAGenerator instance
    NFAGenerator nfaGenerator;

    // Call the regexToNFA function
    NFA *nfa = nfaGenerator.regexToNFA(regex);

    // Assert that the nfa is not null
    ASSERT_NE(nfa, nullptr);

    // Assert that the start state is not null
    ASSERT_NE(nfa->getStartState(), nullptr);

    // Assert that the end state is not null
    ASSERT_NE(nfa->getEndState(), nullptr);

    // Assert that it has 3 end states
    ASSERT_EQ(nfa->getEndStates().size(), 1);
}

TEST_F(NFAGeneratorFixture, SimpleRegexToNFA_ValidInput_ReturnsNFA_1) {
    std::string regex = "a a*|b b*";

    // Create an NFAGenerator instance
    NFAGenerator nfaGenerator;

    // Call the regexToNFA function
    NFA *nfa = nfaGenerator.regexToNFA(regex);

    // Assert that the nfa is not null
    ASSERT_NE(nfa, nullptr);

    // Assert that the start state is not null
    ASSERT_NE(nfa->getStartState(), nullptr);

    // Assert that the end state is not null
    ASSERT_NE(nfa->getEndState(), nullptr);

    // Assert that it has 1 transition from the start state
    ASSERT_EQ(nfa->getStartState()->getTransitions().size(), 1);

    // Assert that it has 1 end state
    ASSERT_EQ(nfa->getEndStates().size(), 1);
}

TEST_F(NFAGeneratorFixture, SimpleRegexToNFA_ValidInput_ReturnsNFA_2) {
    std::string regex = "(a|b)* a b b";

    // Create an NFAGenerator instance
    NFAGenerator nfaGenerator;

    // Call the regexToNFA function
    NFA *nfa = nfaGenerator.regexToNFA(regex);

    // Assert that the nfa is not null
    ASSERT_NE(nfa, nullptr);

    // Assert that the start state is not null
    ASSERT_NE(nfa->getStartState(), nullptr);

    // Assert that the end state is not null
    ASSERT_NE(nfa->getEndState(), nullptr);

    // Assert that it has 1 transition from the start state
    ASSERT_EQ(nfa->getStartState()->getTransitions().size(), 1);

    // Assert that it has 1 end state
    ASSERT_EQ(nfa->getEndStates().size(), 1);
}
