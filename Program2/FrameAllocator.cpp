/* 
 * File:   FrameAllocator.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 * 
 * Created on January 30, 2019, 3:24 PM
 */

#include "FrameAllocator.h"

using namespace std;
    
FrameAllocator::FrameAllocator(int frameNumber) {
    //Initialize "system memory" and fill with zeros
    //0x4000 = 16KB - constant given page frame size
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
    uint32_t total = frameNumber;
    memcpy(&mem.at(PAGE_FRAMES_TOTAL), &total, sizeof(total));
    
    //Number of frames already allocated
    uint32_t available = frameNumber - 1;
    memcpy(&mem.at(PAGE_FRAMES_AVAILABLE), &available, sizeof(available));
    
    //Address of first available frame
    uint32_t headAddr = 0x4000;
    memcpy(&mem.at(AVAILABLE_LIST_HEAD), &headAddr, sizeof(headAddr));
}

bool FrameAllocator::Allocate(uint32_t count, vector<uint32_t> &page_frames) {
    if (count > get_available()) return false;
    
    //Get original head addr
    uint32_t headAddr;
    memcpy(&headAddr, &mem.at(AVAILABLE_LIST_HEAD), sizeof(headAddr));
    
    //Move addr to allocated vector and get next addr
    for (int i = 0; i < count; i++) {
        uint32_t next;
        
        //Move addr to allocated page frames
        page_frames.push_back(headAddr);
        
        //Store next addr in temp variable
        memcpy(&next, &mem.at(headAddr), sizeof(next));
        
        //Zero previous data
        for (uint32_t j = headAddr; j < headAddr + 4; j++) {
            uint8_t val = 0;
            mem->movb(j, &val);
        }
        
        //Update headAddr to next addr
        headAddr = next;
    }
    
    //Update head pointer
    memcpy(&mem.at(AVAILABLE_LIST_HEAD), &headAddr, sizeof(headAddr));
    
    //Update pages available
    uint32_t available = get_available() - count;
    memcpy(&mem.at(PAGE_FRAMES_AVAILABLE), &available, sizeof(available));
    
    return true;
}

bool FrameAllocator::Release(uint32_t count, vector<uint32_t>& page_frames) {
    if (count > page_frames.size()) return false;

    //Get original head addr
    uint32_t headAddr;
    memcpy(&headAddr, &mem.at(AVAILABLE_LIST_HEAD), sizeof(headAddr));
    
    //Remove addr from unallocated list and update head
    for (int i = 0; i < count; i++) {
        //Pop address from allocated vector
        uint32_t newHead = page_frames.back();
        page_frames.pop_back();
        
        //Store last head addr in current head
        memcpy(&mem.at(newHead), &headAddr, sizeof(headAddr));
        
        //Update headAddr to current head
        headAddr = newHead;
    }

    //Update head pointer
    memcpy(&mem.at(AVAILABLE_LIST_HEAD), &headAddr, sizeof(headAddr));
    
    //Update pages available
    uint32_t available = get_available() + count;
    memcpy(&mem.at(PAGE_FRAMES_AVAILABLE), &available, sizeof(available));
    
    return true;
}

uint32_t FrameAllocator::get_available() const {
    uint32_t available;
    memcpy(&available, &mem.at(PAGE_FRAMES_AVAILABLE), sizeof(available));
    
    return available;
}

string FrameAllocator::get_available_list_string() const {
    uint32_t addr;
    memcpy(&addr, &mem.at(AVAILABLE_LIST_HEAD), sizeof(addr));
    
    return build_string(addr);
}

string FrameAllocator::build_string(uint32_t addr) const {
    ostringstream ostring;
    
    //If current addr is not 0xFFFFFFFF, then we can recurse
    if (addr != 0xFFFFFFFF) {
        uint32_t next;
        memcpy(&next, &mem.at(addr), sizeof(next));
        
        ostring << " " << hex << addr <<build_string(next);
    }
    
    return ostring.str();
}

FrameAllocator::~FrameAllocator() {
}
