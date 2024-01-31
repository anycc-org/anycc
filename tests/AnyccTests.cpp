#include <gtest/gtest.h>
#include "Analyzer.h"
#include "Lex.h"
#include "Parser.h"

class AnyccFixture : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};


TEST_F(AnyccFixture, ParserLexIntegration_JavaRules_ValidProgram) {
    std::string program_file_name = "../../tests/test_files/program_test_1_1.txt";
    std::string rules_file_name = "../../tests/test_files/rules_test_1.txt";
    std::string cfg_file_name = "../../tests/test_files/cfg_test_1.txt";

    auto lex = new Lex(rules_file_name, program_file_name);
    ASSERT_NO_THROW(lex->buildLex());

    auto parser = new Parser(cfg_file_name, lex);
    ASSERT_NO_THROW(parser->buildParser());
    ASSERT_NO_THROW(parser->parseProgram());

    lex->~Lex();
}

TEST_F(AnyccFixture, ParserLexIntegration_CFG2_ValidProgram) {
    std::string program_file_name = "../../tests/test_files/program_test_2.txt";
    std::string rules_file_name = "../../tests/test_files/rules_test.txt";
    std::string cfg_file_name = "../../tests/test_files/cfg_test_2.txt";

    auto lex = new Lex(rules_file_name, program_file_name);
    ASSERT_NO_THROW(lex->buildLex());

    auto parser = new Parser(cfg_file_name, lex);
    ASSERT_NO_THROW(parser->buildParser());
    ASSERT_NO_THROW(parser->parseProgram());

    lex->~Lex();
}

TEST_F(AnyccFixture, ParserLexIntegration_CFG3_ValidProgram) {
    std::string program_file_name = "../../tests/test_files/program_test_3.txt";
    std::string rules_file_name = "../../tests/test_files/rules_test.txt";
    std::string cfg_file_name = "../../tests/test_files/cfg_test_3.txt";

    auto lex = new Lex(rules_file_name, program_file_name);
    ASSERT_NO_THROW(lex->buildLex());

    auto parser = new Parser(cfg_file_name, lex);
    ASSERT_NO_THROW(parser->buildParser());
    ASSERT_NO_THROW(parser->parseProgram());

    lex->~Lex();
}

TEST_F(AnyccFixture, ParserLexIntegration_CFG4_ValidProgram) {
    std::string program_file_name = "../../tests/test_files/program_test_4.txt";
    std::string rules_file_name = "../../tests/test_files/rules_test.txt";
    std::string cfg_file_name = "../../tests/test_files/cfg_test_4.txt";

    auto lex = new Lex(rules_file_name, program_file_name);
    ASSERT_NO_THROW(lex->buildLex());

    auto parser = new Parser(cfg_file_name, lex);
    ASSERT_NO_THROW(parser->buildParser());
    ASSERT_NO_THROW(parser->parseProgram());

    lex->~Lex();
}

TEST_F(AnyccFixture, ParserLexIntegration_CFG5_ValidProgram) {
    std::string program_file_name = "../../tests/test_files/program_test_5.txt";
    std::string rules_file_name = "../../tests/test_files/rules_test.txt";
    std::string cfg_file_name = "../../tests/test_files/cfg_test_5.txt";

    auto lex = new Lex(rules_file_name, program_file_name);
    ASSERT_NO_THROW(lex->buildLex());

    auto parser = new Parser(cfg_file_name, lex);
    ASSERT_NO_THROW(parser->buildParser());
    ASSERT_NO_THROW(parser->parseProgram());

    lex->~Lex();
}

TEST_F(AnyccFixture, ParserLexIntegration_CFG6_ValidProgram) {
    std::string program_file_name = "../../tests/test_files/program_test_6.txt";
    std::string rules_file_name = "../../tests/test_files/rules_test.txt";
    std::string cfg_file_name = "../../tests/test_files/cfg_test_6.txt";

    auto lex = new Lex(rules_file_name, program_file_name);
    ASSERT_NO_THROW(lex->buildLex());

    auto parser = new Parser(cfg_file_name, lex);
    ASSERT_NO_THROW(parser->buildParser());
    ASSERT_NO_THROW(parser->parseProgram());

    lex->~Lex();
}