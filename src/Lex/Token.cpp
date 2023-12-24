#include <Lex/Token.h>

Token::Token() = default;

Token::Token(std::string *key, std::string *value) {
    this->key = key;
    this->value = value;
}

Token::Token(std::string *key, std::string *value, Position *position) {
    this->key = key;
    this->value = value;
    this->position = position;
}

Token::~Token() {
    delete key;
    delete value;
    delete position;
}

std::string *Token::getKey() const {
    return key;
}

std::string *Token::getValue() const {
    return value;
}

Position *Token::getPosition() const {
    return position;
}


