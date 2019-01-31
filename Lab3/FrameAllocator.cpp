/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FrameAllocator.cpp
 * Author: elisabethbristol
 * 
 * Created on January 30, 2019, 3:24 PM
 */

#include "FrameAllocator.h"

FrameAllocator::FrameAllocator(int frameNumber) {
    
    //size the mem vector to the appropriate number of frames and fill with 0's
    //TODO: needs to be multiplied by page frame size
    mem.resize(frameNumber * 16384);
    for(int i = 0; i < mem.size(); i++){
        if (i % 16384 && i != 0) {
            mem.at(i) = i + 16380;
        } else {
            mem.at(i) = 0;
        }
    }
    
    //TODO: Debugger code, should print address of next frame
    for(int i = 0; i < mem.size(); i += 16384) {
        std::cout << (int) mem.at(i) << "\n";
    }
}

//explicit empty destructor
FrameAllocator::~FrameAllocator() {
}

