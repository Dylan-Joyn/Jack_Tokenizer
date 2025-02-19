#include "XMLWriter.h"

XMLWriter::XMLWriter(const std::string& filename) : outFile(filename) {
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not open output file: " + filename);
    }
}

XMLWriter::~XMLWriter() {
    if (outFile.is_open()) {
        outFile.close();
    }
}

void XMLWriter::writeHeader() {
    outFile << "<tokens>" << std::endl;
}

void XMLWriter::writeFooter() {
    outFile << "</tokens>" << std::endl;
}

void XMLWriter::writeToken(const Token& token) {
    std::string tokenType = Token::typeToString(token.getType());
    outFile << "<" << tokenType << "> " 
            << token.getValue()
            << " </" << tokenType << ">" << std::endl;
}
