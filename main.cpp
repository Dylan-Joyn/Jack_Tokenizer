#include <iostream>
#include "JackTokenizer.h"
#include "XMLWriter.h"

std::string getOutputFilename(const std::string& inputFilename) {
    return inputFilename.substr(0, inputFilename.find_last_of('.')) + "T.xml";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.jack>" << std::endl;
        return 1;
    }

    try {
        std::string inputFile = argv[1];
        std::string outputFile = getOutputFilename(inputFile);
        
        JackTokenizer tokenizer(inputFile);
        XMLWriter writer(outputFile);
        
        writer.writeHeader();
        
        while (tokenizer.hasMoreTokens()) {
            Token token = tokenizer.getNextToken();
            writer.writeToken(token);
        }
        
        writer.writeFooter();
        
        std::cout << "Tokenization complete. Output written to " << outputFile << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}