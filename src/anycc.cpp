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
  // Print first_sets
  const auto& first_sets = grammarChecker.getFirstSets();
  std::cout << "First Sets:\n";
  for (const auto& entry : first_sets) {
    const std::string&non_terminal = entry.first;
    const std::set<std::string>&first_set = entry.second;

    std::cout << non_terminal << ": { ";
    for (std::string symbol : first_set) {
      std::cout << symbol << ' ';
    }
    std::cout << "}\n";
  }

  // Print followSets
  const auto& followSets = grammarChecker.getFollowSets();
  std::cout << "\nFollow Sets:\n";
  for (const auto& entry : followSets) {
    const std::string&non_terminal = entry.first;
    const std::set<std::string>&follow_set = entry.second;

    std::cout << non_terminal << ": { ";
    for (std::string symbol : follow_set) {
      std::cout << symbol << ' ';
    }
    std::cout << "}\n";
  }
  return 0;
}
