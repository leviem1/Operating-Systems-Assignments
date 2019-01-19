/* 
 * File:   Process.cpp
 * Author: Levi Muniz
 * 
 * Created on January 18, 2019, 4:44 PM
 */

#include "Process.h"

Process::Process(const std::string &filename) {
    file = new std::fstream();
    
    file->open(filename, std::fstream::in);

    if (!file || !file->is_open()) {
        throw std::runtime_error("File could not be opened");
    }
 }

Process::~Process() {
    if (file && file->is_open()) {
        file->close();
    }
    
    delete file;
    file = nullptr;
}