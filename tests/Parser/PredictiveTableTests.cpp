#include <gtest/gtest.h>
#include "PredictiveTable.h"
#include "Production.h"

class PredictiveTableFixture : public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {
    }
};

TEST_F(PredictiveTableFixture, LookUp_ValidEntries_CFG2_CheckCorrectness) {
    // Initialize the first and follow sets
    Production s_epsilon = Production();
    s_epsilon.nonTerminal = "S";
    s_epsilon.productions.push_back({"Epsilon"});

    Production s_a = Production();
    s_a.nonTerminal = "S";
    s_a.productions.push_back({"A", "b", "S"});

    Production s_c = Production();
    s_c.nonTerminal = "S";
    s_c.productions.push_back({"A", "b", "S"});

    Production s_e = Production();
    s_e.nonTerminal = "S";
    s_e.productions.push_back({"e"});

    Production a_a = Production();
    a_a.nonTerminal = "A";
    a_a.productions.push_back({"a"});

    Production a_c = Production();
    a_c.nonTerminal = "A";
    a_c.productions.push_back({"c", "A", "d"});

    std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> first_sets = {
            {"S", {{"Epsilon", s_epsilon}, {"a", s_a}, {"c", s_c}, {"e", s_e}}},
            {"A", {{"a",       a_a},       {"c", a_c}}}
    };

    std::unordered_map<std::string, std::set<std::string>> follow_sets = {
            {"A", {"b", "d"}},
            {"S", {"$"}}
    };

    std::set<std::string> non_terminals = {
            "S", "A"
    };

    // Create an instance of the PredictiveTable class
    auto predictive_table = new PredictiveTable(first_sets, follow_sets, non_terminals);

    // Build the predictive table
    predictive_table->buildPredictiveTable();

    // Look up specific entries in the predictive table and verify their correctness
    const CellValue *cell_value = predictive_table->lookUp("S", "a");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::VALID_PRODUCTION);
    ASSERT_EQ(cell_value->getProduction().nonTerminal, "S");
    ASSERT_EQ(cell_value->getProduction().productions[0][0], "A");
    ASSERT_EQ(cell_value->getProduction().productions[0][1], "b");
    ASSERT_EQ(cell_value->getProduction().productions[0][2], "S");

    cell_value = predictive_table->lookUp("A", "d");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::SYNC);
    ASSERT_EQ(cell_value->getProduction().nonTerminal, "");

    cell_value = predictive_table->lookUp("S", "$");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::VALID_PRODUCTION);
    ASSERT_EQ(cell_value->getProduction().nonTerminal, "S");
    ASSERT_EQ(cell_value->getProduction().productions[0][0], "Epsilon");

    cell_value = predictive_table->lookUp("A", "$");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::EMPTY);
    ASSERT_EQ(cell_value->getProduction().nonTerminal, "");
    ASSERT_TRUE(cell_value->getProduction().productions.empty());
}

TEST_F(PredictiveTableFixture, LookUp_ValidEntries_CFG3_CheckCorrectness) {
    // Initialize the first and follow sets
    Production v_epsilon = Production();
    v_epsilon.nonTerminal = "V";
    v_epsilon.productions.push_back({"Epsilon"});

    Production v_v = Production();
    v_v.nonTerminal = "V";
    v_v.productions.push_back({"v", "V"});

    Production u_epsilon = Production();
    u_epsilon.nonTerminal = "U";
    u_epsilon.productions.push_back({"Epsilon"});

    Production u_u = Production();
    u_u.nonTerminal = "U";
    u_u.productions.push_back({"u", "U"});

    Production t_epsilon = Production();
    t_epsilon.nonTerminal = "T";
    t_epsilon.productions.push_back({"Epsilon"});

    Production t_t = Production();
    t_t.nonTerminal = "T";
    t_t.productions.push_back({"V", "t", "T"});

    Production t_v = Production();
    t_v.nonTerminal = "T";
    t_v.productions.push_back({"V", "t", "T"});

    Production s = Production();
    s.nonTerminal = "S";
    s.productions.push_back({"R", "T"});

    Production r_epsilon = Production();
    r_epsilon.nonTerminal = "R";
    r_epsilon.productions.push_back({"Epsilon"});

    Production r_s = Production();
    r_s.nonTerminal = "R";
    r_s.productions.push_back({"s", "U", "R", "b"});


    std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> first_sets = {
            {"V", {{"Epsilon", v_epsilon}, {"v", v_v}}},
            {"U", {{"Epsilon", u_epsilon}, {"u", u_u}}},
            {"T", {{"Epsilon", t_epsilon}, {"t", t_t}, {"v", t_v}}},
            {"S", {{"Epsilon", s},         {"s", s},   {"t", s}, {"v", s}}},
            {"R", {{"Epsilon", r_epsilon}, {"s", r_s}}}
    };

    std::unordered_map<std::string, std::set<std::string>> follow_sets = {
            {"V", {"t"}},
            {"U", {"b", "s"}},
            {"T", {"$"}},
            {"R", {"$", "b", "t", "v"}},
            {"S", {"$"}}
    };

    std::set<std::string> non_terminals = {
            "V", "U", "T", "S", "R"
    };

    // Create an instance of the PredictiveTable class
    auto predictive_table = new PredictiveTable(first_sets, follow_sets, non_terminals);

    // Build the predictive table
    predictive_table->buildPredictiveTable();

    // Look up specific entries in the predictive table and verify their correctness
    const CellValue *cell_value = predictive_table->lookUp("S", "$");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::VALID_PRODUCTION);
    ASSERT_EQ(cell_value->getProduction().nonTerminal, "S");
    ASSERT_EQ(cell_value->getProduction().productions[0][0], "R");
    ASSERT_EQ(cell_value->getProduction().productions[0][1], "T");

    cell_value = predictive_table->lookUp("U", "u");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::VALID_PRODUCTION);
    ASSERT_EQ(cell_value->getProduction().productions[0][0], "u");
    ASSERT_EQ(cell_value->getProduction().productions[0][1], "U");

    cell_value = predictive_table->lookUp("R", "s");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::VALID_PRODUCTION);
    ASSERT_EQ(cell_value->getProduction().nonTerminal, "R");
    ASSERT_EQ(cell_value->getProduction().productions[0][0], "s");
    ASSERT_EQ(cell_value->getProduction().productions[0][1], "U");
    ASSERT_EQ(cell_value->getProduction().productions[0][2], "R");
    ASSERT_EQ(cell_value->getProduction().productions[0][3], "b");

    cell_value = predictive_table->lookUp("T", "u");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::EMPTY);
    ASSERT_EQ(cell_value->getProduction().nonTerminal, "");
    ASSERT_TRUE(cell_value->getProduction().productions.empty());
}

TEST_F(PredictiveTableFixture, LookUp_ValidEntries_CFG4_CheckCorrectness) {
    // Initialize the productions for the CFG
    Production e = Production();
    e.nonTerminal = "E";
    e.productions.push_back({"T", "E1"});

    Production e1_add = Production();
    e1_add.nonTerminal = "E1";
    e1_add.productions.push_back({"add", "T", "E1"});

    Production e1_epsilon = Production();
    e1_epsilon.nonTerminal = "E1";
    e1_epsilon.productions.push_back({"Epsilon"});

    Production t = Production();
    t.nonTerminal = "T";
    t.productions.push_back({"F", "T1"});

    Production t1_mul = Production();
    t1_mul.nonTerminal = "T1";
    t1_mul.productions.push_back({"mul", "F", "T1"});

    Production t1_epsilon = Production();
    t1_epsilon.nonTerminal = "T1";
    t1_epsilon.productions.push_back({"Epsilon"});

    Production f_exp = Production();
    f_exp.nonTerminal = "F";
    f_exp.productions.push_back({"(", "E", ")"});

    Production f_id = Production();
    f_id.nonTerminal = "F";
    f_id.productions.push_back({"id"});

    // Define expected first and follow sets
    std::unordered_map<std::string, std::set<std::pair<std::string, Production>, CompareFirst>> first_sets = {
            {"T",  {{"(",       t},          {"id",  t}}},
            {"T1", {{"Epsilon", t1_epsilon}, {"mul", t1_mul}}},
            {"E",  {{"(",       e},          {"id",  e}}},
            {"E1", {{"Epsilon", e1_epsilon}, {"add", e1_add}}},
            {"F",  {{"(",       f_exp},      {"id",  f_id}}}
    };

    std::unordered_map<std::string, std::set<std::string>> follow_sets = {
            {"T",  {"$", ")", "add"}},
            {"F",  {"$", ")", "add", "mul"}},
            {"T1", {"$", ")", "add"}},
            {"E1", {"$", ")"}},
            {"E",  {"$", ")"}}
    };

    std::set<std::string> non_terminals = {"E", "E1", "T", "T1", "F"};

    // Create an instance of the PredictiveTable class
    auto predictive_table = new PredictiveTable(first_sets, follow_sets, non_terminals);
    // Build the predictive table
    predictive_table->buildPredictiveTable();

    // Look up specific entries in the predictive table and verify their correctness
    const CellValue *cell_value = predictive_table->lookUp("E", "$");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::SYNC);

    cell_value = predictive_table->lookUp("E1", "add");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::VALID_PRODUCTION);
    ASSERT_EQ(cell_value->getProduction().productions[0][0], "add");
    ASSERT_EQ(cell_value->getProduction().productions[0][1], "T");
    ASSERT_EQ(cell_value->getProduction().productions[0][2], "E1");

    cell_value = predictive_table->lookUp("F", "(");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::VALID_PRODUCTION);
    ASSERT_EQ(cell_value->getProduction().nonTerminal, "F");
    ASSERT_EQ(cell_value->getProduction().productions[0][0], "(");
    ASSERT_EQ(cell_value->getProduction().productions[0][1], "E");
    ASSERT_EQ(cell_value->getProduction().productions[0][2], ")");

    cell_value = predictive_table->lookUp("T1", "id");
    ASSERT_EQ(cell_value->getPredictiveTableEntryType(), ParsingTableEntryType::EMPTY);
    ASSERT_EQ(cell_value->getProduction().nonTerminal, "");
    ASSERT_TRUE(cell_value->getProduction().productions.empty());
}

