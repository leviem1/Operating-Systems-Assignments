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

FrameAllocator::FrameAllocator(int frameNumber ) {
    
    //size the mem vector to the appropriate number of frames and fill with 0's
    //TODO: needs to be multiplied by page frame size
    mem.resize(frameNumber);
    for(int i = 0; i < mem.size() - 1; i++){
        mem.at(i) = 0;
    }
}

//explicit empty destructor
FrameAllocator::~FrameAllocator() {
}

