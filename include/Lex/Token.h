#ifndef ANYCC_TOKEN_H
#define ANYCC_TOKEN_H

#include <string>

struct Position {
    int line_number;
    int column_number;

    Position(int line_number, int column_number) : line_number(line_number), column_number(column_number) {}

    Position() = default;
};

class Token {
public:

    Token();

    Token(std::string *key, std::string *value);

    Token(std::string *key, std::string *value, Position *position);

    ~Token();

    std::string *getKey() const;

    std::string *getValue() const;

    Position *getPosition() const;

private:
    std::string *key{};
    std::string *value{};
    Position *position{};
};

#endif //ANYCC_TOKEN_H