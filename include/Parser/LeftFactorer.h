#ifndef LEFT_FACTORER_H
#define LEFT_FACTORER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_map>
#include <set>


class LeftFactorer {
public:
    static std::unordered_map<std::string, std::vector<std::vector<std::string>>> leftFactor(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar);

private:
    static std::pair<std::vector<std::string>, std::set<std::vector<std::string>>> getLongestCommonPrefix(const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string non_terminal);

    static std::string leftFactorProduction(std::unordered_map<std::string, std::vector<std::vector<std::string>>>& grammar, std::string non_terminal, size_t factored_count);


};


#endif // LEFT_FACTORER_H