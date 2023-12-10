#include "Lex/NFAState.h"
#include <map>
#include "Lex/Lex.h"
//phase one main
//int main() {
//    auto *rules_file_name = new std::string("../rules.txt");
//    auto *program_file_name = new std::string("../program.txt");
//
//    Lex *lex = new Lex(rules_file_name, program_file_name);
//    lex->buildLex();
//
//    lex->getAllTokensAndCreateOutputFile();
//    lex->printSymbolTable();
//}

#include "Parser/FirstAndFollowGenerator.h"
#include "constants.h"
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
int main() {
  std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
      {"S", {{"T", "E'"}}},
      {"E'", {{"+", "T", "E'"}, {EPSILON}}},
      {"T", {{"F", "T'"}}},
      {"T'", {{"*", "F", "T'"}, {EPSILON}}},
      {"F", {{"(", "S", ")"}, {"id"}}}
  };

  char x='\1';
  cout << x;
  FirstAndFollowGenerator grammarChecker(grammar);
  grammarChecker.isLL1Grammar();
  // Print firstSets
  const auto& firstSets = grammarChecker.getFirstSets();
  cout << "First Sets:\n";
  for (const auto& entry : firstSets) {
    const string& nonTerminal = entry.first;
    const set<string>& firstSet = entry.second;

    cout << nonTerminal << ": { ";
    for (string symbol : firstSet) {
      cout << symbol << ' ';
    }
    cout << "}\n";
  }

  // Print followSets
  const auto& followSets = grammarChecker.getFollowSets();
  cout << "\nFollow Sets:\n";
  for (const auto& entry : followSets) {
    const string& nonTerminal = entry.first;
    const set<string>& followSet = entry.second;

    cout << nonTerminal << ": { ";
    for (string symbol : followSet) {
      cout << symbol << ' ';
    }
    cout << "}\n";
  }
  return 0;
}
