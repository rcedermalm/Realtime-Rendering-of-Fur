#include <FileReader.hpp>

#include <iostream>
#include <fstream>

const std::string FileReader::ReadFromFile(std::string fileName) {
    std::ifstream ifs(fileName.c_str());
    std::string out;

    if (ifs.is_open()) {
        std::string s;
        while (std::getline(ifs, s)) {
            out += s + "\n";
        }
    } else {
        std::cerr << "Could not open file" << std::endl;
    }

    ifs.close();
    return out;
}