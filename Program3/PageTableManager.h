
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
    
    /**
     * PTM constructor
     * @param mem - the one MMU instance for whole trace
     * @param fa - the frame allocator that needs to be maintained for the 
     * whole trace
     */
    PageTableManager(mem::MMU &mem, FrameAllocator &fa);
  
    //explicit deletes
    PageTableManager(const PageTableManager &other) = delete;  // no copy constructor
    PageTableManager(PageTableManager &&other) = delete;       // no move constructor
    PageTableManager operator=(const PageTableManager &other) = delete;  // no copy assign
    PageTableManager operator=(PageTableManager &&other) = delete;       // no move assign

    /**
     * buildUserPageTable - builds a user page table for the process
     * @return - the place in physical memory where we built the table
     */
    mem::Addr buildUserPageTable();
    
    /**
     * allocate - calls the frame allocator if needed and then
     * manipulates the thing in physical memory so it can be 
     * put in the correct spot in the page table
     * @param count - the amount of frames to allocate
     * @param page_table_base - the base of the page table for the process
     * @param vaddr - allocate the frames starting at this address
     * @return - bool of whether the memory was successfully allocated or not
     */
    int allocate(std::uint32_t count, mem::Addr page_table_base, std::uint32_t vaddr);
    
    /**
     * setWritable - sets present entries writable bit to status to adjust
     * permission levels
     * @param pmcb - the pmcb for the process involved
     * @param vaddr - start adjusting permissions at this virtual address
     * @param count - check this number of entries
     * @param status - set the writeable bit to this status. if true -> 1 
     * otherwise -> 0
     */
    void setWritable(mem::PMCB *pmcb, std::uint32_t vaddr, int count, bool status);

    void releaseAll(mem::PMCB *pmcb);

    uint32_t getAvailable() {
        return fa->get_available();
    }
   
    ~PageTableManager() = default;
    
private:
    void findAllocatedAddresses(mem::Addr page_table_base, std::vector<mem::Addr> &addresses);
    mem::MMU *mem;
    FrameAllocator *fa;
};

#endif /* PAGETABLEMANAGER_H */

