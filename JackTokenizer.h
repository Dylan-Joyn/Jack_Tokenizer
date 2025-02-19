#ifndef JACK_TOKENIZER_H
#define JACK_TOKENIZER_H
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Token.h"

class JackTokenizer{
    public:
    JackTokenizer(const std::string& filename);
    bool hasMoreTokens();
    Token getNextTokens();
    std::string getXMLEntity(const std::string& value);

    private:
    std::string input;
    size_t currentPos;

    bool isWhitespace(char c);
    bool checkForComment();
    void skipSingleLineComment();
    void skipMultiLineComment();

    const std::unordered_set<std::string> keywords;
    const std::unordered_set<char> symbols;
    const std::unordered_map<char, std::string> xmlEntities;

    void skipWhitespaceAndComments();
    void initializeConstants();
    Token handleStringConstant();
    Token handleSymbol();
    Token handleNumber();
    Token handleIdentifierOrKeyword();
};

#endif 