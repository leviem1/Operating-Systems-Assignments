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
#include <cstdint>
#include <vector>

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
    
    void Exec();
    
private:
    std::fstream *file;
    std::uint64_t line;
    std::vector<uint8_t> *mem;
    
    void memsize(int);
    void cmp(int, int, int);
    void fill(int, int, int);
    void dup(int, int, int);
};

#endif /* PROCESS_H */
