#pragma once

#include <iostream>
#include <fstream>

class FileReader {
public:
    /**
     * @brief Read the file skeleton with a function to read as a template
     * Override readTemplate to read the file
     * @param file The file to be read
     */
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

    /**
     * @brief Read the file template, it is to be overridden
     * @param file The file to be read
     */
    virtual void readTemplate(std::ifstream *file) = 0;
};