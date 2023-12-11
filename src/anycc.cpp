#include <map>
#include "Lex/Lex.h"
#include "Parser/FirstAndFollowGenerator.h"
#include "constants.h"
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
// Function to parse the CFG input file
std::unordered_map<std::string, std::vector<std::vector<std::string>>> parseCFGInput(const std::string& filename) {
  std::ifstream inputFile(filename);
  std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar;

  if (inputFile.is_open()) {
    std::string line;
    std::string currentNonTerminal;

    while (std::getline(inputFile, line)) {
      // Skip comments and empty lines
      if (line.empty() || line[0] == '#')
        continue;

      // Find the position of ::= in the line
      size_t arrowPos = line.find("::=");
      if (arrowPos != std::string::npos) {
        // Extract the non-terminal before ::= as the current non-terminal
        currentNonTerminal = line.substr(0, arrowPos);

        // Tokenize the RHS of the production rule using '|'
        line = line.substr(arrowPos + 3); // Move past "::="
        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> production;

        while (ss >> token) {
          if (token == "|") {
            // Start a new production for the same non-terminal
            grammar[currentNonTerminal].push_back(production);
            production.clear();
          } else {
            production.push_back(token);
          }
        }

        // Add the last production to the grammar
        grammar[currentNonTerminal].push_back(production);
      }
    }

    inputFile.close();
  } else {
    std::cerr << "Unable to open file: " << filename << std::endl;
  }

  return grammar;
}

int main() {
  std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar =parseCFGInput("../CFG.txt");
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
