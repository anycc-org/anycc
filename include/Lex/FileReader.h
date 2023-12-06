#pragma once

#include <iostream>
#include <fstream>

class FileReader {
public:
    void readFile(std::ifstream *file) {
        std::string line;
        if (file->is_open()) {
            readTemplate(file);
            file->close();
            delete file;
        } else {
            std::cout << "Unable to open file";
        }
    }

    virtual void readTemplate(std::ifstream *file) = 0;
};