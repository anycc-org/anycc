#pragma once

#include <string>

class Token {
public:

    Token();

    Token(std::string *key, std::string *value);

    ~Token();

    std::string *getKey() const;

    std::string *getValue() const;

private:
    std::string *key{};
    std::string *value{};
};