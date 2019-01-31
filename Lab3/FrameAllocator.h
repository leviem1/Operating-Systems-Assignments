/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FrameAllocator.h
 * Author: elisabethbristol
 *
 * Created on January 30, 2019, 3:24 PM
 */

#ifndef FRAMEALLOCATOR_H
#define FRAMEALLOCATOR_H

#include<vector>
#include <iostream>
#include <cstring>
#include <sstream>

class FrameAllocator {
public:
    
    FrameAllocator(int frameNumber);
    
    bool Allocate(std::uint32_t count, std::vector<std::uint32_t> &page_frames);
    
    std::uint32_t get_available() const;
    std::string get_available_list_string() const;
    
    ~FrameAllocator();
    
    //explicit rule of five deletes
    FrameAllocator(const FrameAllocator& other) = delete;
    FrameAllocator(FrameAllocator&& other) = delete;
    FrameAllocator& operator=(const FrameAllocator& other) = delete;
    FrameAllocator& operator=(FrameAllocator&& other) = delete;
    
private:
    std::vector<std::uint8_t> mem;
    //Pointer to PAGE_FRAMES_TOTAL value
    const static std::uint16_t PAGE_FRAMES_TOTAL = 0;
    //Pointer to PAGE_FRAMES_AVAILABLE value
    const static std::uint16_t PAGE_FRAMES_AVAILABLE = 32;
    //Pointer to AVAILABLE_LIST_HEAD value
    const static std::uint16_t AVAILABLE_LIST_HEAD = 64;
    //Recursive function to build available list string
    std::string build_string(uint32_t addr) const;

};

#endif /* FRAMEALLOCATOR_H */

