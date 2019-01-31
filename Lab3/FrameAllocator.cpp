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

using namespace std;
    
FrameAllocator::FrameAllocator(int frameNumber) {
    //Initialize "system memory" and fill with zeros
    //0x4000 = 16KB
    mem.resize(frameNumber * 0x4000, 0);
    
    //Create links by storing address of next free frame in first 32-bits
    for (uint32_t i = 0x4000; i < mem.size(); i += 0x4000) {
        uint32_t next;
        
        //Last value must be 0xFFFFFFFF
        if (i == mem.size() - 0x4000) {
            next = 0xFFFFFFFF;
            memcpy(&mem.at(i), &next, sizeof(next));
        }
        
        //Set value to address of next free frame
        else {
            next = i + 0x4000;
            memcpy(&mem.at(i), &next, sizeof(next));  
        }
    }
    
    //Initialize memory allocator variables (stored directly in mem)
    //Total number of frames in mem
    uint32_t pageFramesTotal = frameNumber;
    memcpy(&mem.at(PAGE_FRAMES_TOTAL),
            &pageFramesTotal, 
            sizeof(pageFramesTotal));
    
    //Number of frames already allocated
    uint32_t pageFramesAvailable = frameNumber - 1;
    memcpy(&mem.at(PAGE_FRAMES_AVAILABLE),
            &pageFramesAvailable,
            sizeof(pageFramesAvailable));
    
    //Address of first available frame
    uint32_t availableListHead = 0x4000;
    memcpy(&mem.at(AVAILABLE_LIST_HEAD),
            &availableListHead,
            sizeof(availableListHead));   
}

bool FrameAllocator::Allocate(uint32_t count, vector<uint32_t>& page_frames) {
    
}

uint32_t FrameAllocator::get_available() const {
    return mem.at(PAGE_FRAMES_AVAILABLE);
}

string FrameAllocator::get_available_list_string() const {
    uint32_t addr;
    memcpy(&addr, &mem.at(AVAILABLE_LIST_HEAD), sizeof(addr));
    
    return build_string(addr);
}

string FrameAllocator::build_string(uint32_t addr) const {
    ostringstream ostring;
    uint32_t next;
    memcpy(&next, &mem.at(addr), sizeof (next));
    
    //Print this address
    ostring << " " << hex << addr;
    
    //If we're at the end of the list, append new line
    if (next != 0xFFFFFFFF) {
        ostring << build_string(next);
    }
    
    return ostring.str();
}

//explicit empty destructor
FrameAllocator::~FrameAllocator() {
}
