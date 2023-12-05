#pragma once

#include <iostream>
#include <fstream>

class FileReader {
public:
    void readFile(std::ifstream *file) {
        std::string line;
        if (file->is_open()) {
            while (getline(*file, line)) {
                parseLine(line);
            }
            file->close();
            delete file;
        } else {
            std::cout << "Unable to open file";
        }
    }

    virtual void parseLine(std::string &line) = 0;
};