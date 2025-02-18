#ifndef TOKEN.H
#define TOKEN.H
#include <string>

class Token{
    public:
    enum class Type{
        KEYWORD,
        SYMBOL,
        IDENTIFIER,
        INT_CONST,
        STRING_CONST
    };


    Token(Type type, const std::string& value);
    Type getType() const;
    std::string getValue() const;
    static std::string typeToString(Type type);

    private:
    Type type;
    std::string value;



};

#endif //TOKEN.H