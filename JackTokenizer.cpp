#include "JackTokenizer.h"
#include "Token.h"
#include <fstream>
#include <stdexcept>
#include <cctype>

JackTokenizer::JackTokenizer(const std::string& filename) : currentPos(0){
    std::ifstream file(filename);
    if (!file.is_open()){
        throw std::runtime_error("Could not open file: "+filename);

    }

    std::string line;
    while(std::getline(file, line)){
        input += line + '\n';
    }

    initializeConstants();
}

void JackTokenizer::initializeConstants() {
    const_cast<std::unordered_set<std::string>&>(keywords) = {
        "class", "constructor", "function", "method", "field", "static",
        "var", "int", "char", "boolean", "void", "true", "false", "null",
        "this", "let", "do", "if", "else", "while", "return"
    };

    const_cast<std::unordered_set<char>&>(symbols) = {
        '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', 
        '-', '*', '/', '&', '|', '<', '>', '=', '~'
    };

    const_cast<std::unordered_map<char, std::string>&>(xmlEntities) = {
        {'<', "&lt;"},
        {'>', "&gt;"},
        {'&', "&amp;"},
        {'"', "&quot;"}
    };
}

bool JackTokenizer::isWhitespace(char c) {
    return std::isspace(static_cast<unsigned char>(c));
}

bool JackTokenizer::checkForComment() {
    if (currentPos + 1 >= input.length()) {
        return false;
    }

    // Check for single-line comment
    if (input[currentPos] == '/' && input[currentPos + 1] == '/') {
        skipSingleLineComment();
        return true;
    }

    // Check for multi-line comment
    if (input[currentPos] == '/' && input[currentPos + 1] == '*') {
        skipMultiLineComment();
        return true;
    }

    return false;
}

void JackTokenizer::skipSingleLineComment() {
    // Move past the //
    currentPos += 2;
    
    // Skip until end of line or end of file
    while (currentPos < input.length() && input[currentPos] != '\n') {
        currentPos++;
    }
}

void JackTokenizer::skipMultiLineComment() {
    // Move past the /*
    currentPos += 2;
    
    bool isDocumentation = (currentPos < input.length() && input[currentPos] == '*');
    
    // Skip until closing */ is found
    while (currentPos + 1 < input.length()) {
        if (input[currentPos] == '*' && input[currentPos + 1] == '/') {
            currentPos += 2;  // Move past the */
            return;
        }
        currentPos++;
    }
    
    // If we reach here without finding */, the comment was not properly closed
    throw std::runtime_error("Unclosed multi-line comment detected");
}

void JackTokenizer::skipWhitespaceAndComments() {
    while (currentPos < input.length()) {
        // Skip whitespace
        while (currentPos < input.length() && isWhitespace(input[currentPos])) {
            currentPos++;
        }
        
        // If we can't find any more comments, break
        if (!checkForComment()) {
            break;
        }
    }
}

bool JackTokenizer::hasMoreTokens() {
    skipWhitespaceAndComments();
    return currentPos < input.length();
}

Token JackTokenizer::handleStringConstant() {
    currentPos++; // Skip opening quote
    std::string str;
    while (currentPos < input.length() && input[currentPos] != '"') {
        str += input[currentPos++];
    }
    currentPos++; // Skip closing quote
    return Token(Token::Type::STRING_CONST, str);
}

Token JackTokenizer::handleSymbol() {
    std::string symbolStr(1, input[currentPos++]);
    return Token(Token::Type::SYMBOL, symbolStr);
}

Token JackTokenizer::handleNumber() {
    std::string num;
    while (currentPos < input.length() && std::isdigit(input[currentPos])) {
        num += input[currentPos++];
    }
    return Token(Token::Type::INT_CONST, num);
}

Token JackTokenizer::handleIdentifierOrKeyword() {
    std::string word;
    while (currentPos < input.length() && 
           (std::isalnum(input[currentPos]) || input[currentPos] == '_')) {
        word += input[currentPos++];
    }
    
    if (keywords.find(word) != keywords.end()) {
        return Token(Token::Type::KEYWORD, word);
    }
    return Token(Token::Type::IDENTIFIER, word);
}

Token JackTokenizer::getNextToken() {
    skipWhitespaceAndComments();
    
    if (currentPos >= input.length()) {
        throw std::runtime_error("No more tokens");
    }

    char currentChar = input[currentPos];

    // Handle string constants
    if (currentChar == '"') {
        return handleStringConstant();
    }

    // Handle symbols
    if (symbols.find(currentChar) != symbols.end()) {
        return handleSymbol();
    }

    // Handle integer constants
    if (std::isdigit(currentChar)) {
        return handleNumber();
    }

    // Handle keywords and identifiers
    if (std::isalpha(currentChar) || currentChar == '_') {
        return handleIdentifierOrKeyword();
    }

    throw std::runtime_error("Invalid token at position " + std::to_string(currentPos));
}

std::string JackTokenizer::getXMLEntity(const std::string& value) {
    if (value.length() == 1 && xmlEntities.find(value[0]) != xmlEntities.end()) {
        return xmlEntities.at(value[0]);
    }
    return value;
}