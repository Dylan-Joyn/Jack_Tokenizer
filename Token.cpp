#include "Token.h"

Token::Token(Type type, const std::string& value)
: type(type), value(value){}

Token::Type Token::getType() const{
    return type;
}

std::string Token::getValue() const{
    return value;
}

std::string Token::typeToString(Type type){
    switch (type){
        case Type::KEYWORD: return "keyword";
        case Type::SYMBOL: return "symbol";
        case Type::IDENTIFIER: return "identifier";
        case Type::INT_CONST: return "integerConstant";
        case Type::STRING_CONST: return "stringConstant";
        default: return "unknown";
    }
}