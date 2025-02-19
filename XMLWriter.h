#ifndef XML_WRITER_H
#define XML_WRITER_H
#include <string>
#include <fstream>
#include "Token.h"

class XMLWriter {
public:
    XMLWriter(const std::string& filename);
    ~XMLWriter();
    
    void writeToken(const Token& token);
    void writeHeader();
    void writeFooter();

private:
    std::ofstream outFile;
};

#endif