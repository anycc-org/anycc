#include <Lex/Token.h>

Token::Token() {

}

Token::Token(std::string *key, std::string *value) {
    this->key = key;
    this->value = value;
}

Token::~Token() {

}

std::string *Token::getKey() const {
    return key;
}

std::string *Token::getValue() const {
    return value;
}


