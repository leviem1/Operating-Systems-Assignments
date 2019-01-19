/* 
 * File:   Process.h
 * Author: Levi Muniz
 *
 * Created on January 18, 2019, 4:44 PM
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <string>
#include <fstream>

/*
 * 
 */
class Process {
public:
    Process(const std::string &filename);
    Process(const Process& other) = delete;
    Process(Process&& other) = delete;
    Process& operator=(const Process& other) = delete;
    Process& operator=(Process&& other) = delete;
    ~Process();
private:
    std::fstream *file;
};

#endif /* PROCESS_H */
