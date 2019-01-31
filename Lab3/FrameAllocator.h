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

class FrameAllocator {
public:
    
    FrameAllocator(int frameNumber);
    
    virtual ~FrameAllocator();
    
    //explicit rule of five deletes
    FrameAllocator(const FrameAllocator& other) = delete;
    FrameAllocator(FrameAllocator&& other) = delete;
    FrameAllocator& operator=(const FrameAllocator& other) = delete;
    FrameAllocator& operator=(FrameAllocator&& other) = delete;
    
private:
    std::vector<std::uint8_t> mem;

};

#endif /* FRAMEALLOCATOR_H */

