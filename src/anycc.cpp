#include <map>
#include "Lex/Lex.h"
#include "Parser/FirstAndFollowGenerator.h"
#include "constants.h"
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include "Utilities.h"

int main() {
  std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar =Utilities::parseCFGInput("../CFG.txt");
  FirstAndFollowGenerator grammarChecker(grammar);
  grammarChecker.isLL1Grammar();
  // Print firstSets
  const auto& firstSets = grammarChecker.getFirstSets();
  std::cout << "First Sets:\n";
  for (const auto& entry : firstSets) {
    const std::string& nonTerminal = entry.first;
    const std::set<std::string>& firstSet = entry.second;

    std::cout << nonTerminal << ": { ";
    for (std::string symbol : firstSet) {
      std::cout << symbol << ' ';
    }
    std::cout << "}\n";
  }

  // Print followSets
  const auto& followSets = grammarChecker.getFollowSets();
  std::cout << "\nFollow Sets:\n";
  for (const auto& entry : followSets) {
    const std::string& nonTerminal = entry.first;
    const std::set<std::string>& followSet = entry.second;

    std::cout << nonTerminal << ": { ";
    for (std::string symbol : followSet) {
      std::cout << symbol << ' ';
    }
    std::cout << "}\n";
  }
  return 0;
}
