
/* 
 * File:   PageTableManager.h
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on February 25, 2019, 5:19 PM
 */

#ifndef PAGETABLEMANAGER_H
#define PAGETABLEMANAGER_H

#include <MMU.h>
#include "FrameAllocator.h"

class PageTableManager {
public:
    
    PageTableManager(mem::MMU &mem, FrameAllocator &fa);
  
    PageTableManager(const PageTableManager &other) = delete;  // no copy constructor
    PageTableManager(PageTableManager &&other) = delete;       // no move constructor
    PageTableManager operator=(const PageTableManager &other) = delete;  // no copy assign
    PageTableManager operator=(PageTableManager &&other) = delete;       // no move assign

    mem::Addr buildUserPageTable(int vaddr);
    bool allocate(std::uint32_t count, mem::PMCB *pmcb, int vaddr);
   
    ~PageTableManager();
private:
    mem::MMU *mem;
    FrameAllocator *fa;
};

#endif /* PAGETABLEMANAGER_H */

