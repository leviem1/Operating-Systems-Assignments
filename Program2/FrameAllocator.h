/* 
 * File:   FrameAllocator.h
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on January 30, 2019, 3:24 PM
 */

#ifndef FRAMEALLOCATOR_H
#define FRAMEALLOCATOR_H

#include <MMU.h>

#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>

class FrameAllocator {
public:
    /* Constructor for Frame Allocator
     * @param frameNumber - from file line 1, number of frames
     */
    FrameAllocator(int frameNumber);
    
    /* Allocate - allocates spaces in memory - sets allocated bytes to 0
     * @param count - the number of available page frames to allocate
     * @param page frames - which processes table should we be looking at
     * @returns - bool true if success false if not enough free mem to allocate
     */
    bool Allocate(std::uint32_t count, std::vector<std::uint32_t> &page_frames);
    
    /**
     * Release - frees up count amount of mem from back of list
     * @param count - number of page frame address to be released
     * @param page_frames - from which processes table
     * @return - bool. If count > then whole table return false otherwise true
     */
    bool Release(std::uint32_t count, std::vector<std::uint32_t> &page_frames);
    
    /**
     * get_available - getter for available entry
     * @return - the available entry
     */
    std::uint32_t get_available() const;
    
    /**
     * get_available_list_string - getter for available mem as a string
     * @return - a string with all available slots in memory in correct format
     */
    std::string get_available_list_string() const;
    
    /**
     * empty destructor defined for frame allocator
     */
    ~FrameAllocator();
    
    //explicit rule of five deletes
    FrameAllocator(const FrameAllocator& other) = delete;
    FrameAllocator(FrameAllocator&& other) = delete;
    FrameAllocator& operator=(const FrameAllocator& other) = delete;
    FrameAllocator& operator=(FrameAllocator&& other) = delete;
    
private:
    //vector to store memory
    mem::MMU *mem;
    
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

