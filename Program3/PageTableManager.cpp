

/* 
 * File:   PageTableManager.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 * 
 * Created on February 25, 2019, 5:19 PM
 */

#include "PageTableManager.h"

using namespace mem;

PageTableManager::PageTableManager(MMU &mem, FrameAllocator &fa) {
    this->mem = &mem;
    this->fa = &fa;
}

Addr PageTableManager::buildUserPageTable(int vaddress) {
    std::vector<Addr> address;
    fa->Allocate(1, address);

    PageTable page_table;
    Addr pt_offset = (vaddress >> kPageSizeBits) & kPageTableIndexMask;
    page_table.at(pt_offset) = address[0] | kPTE_PresentMask | kPTE_WritableMask;
    mem->movb(address[0], &page_table, kPageTableSizeBytes);

    return address[0];
}

int PageTableManager::allocate(std::uint32_t count, mem::Addr page_table_base, uint32_t vaddr) {
    std::vector<uint32_t> page_frames;
    bool didAlloc = fa->Allocate(count, page_frames);

    for(int i = 0; i < page_frames.size(); i++){
        //calculate the offset that we need to add to the vaddr in the user page table
        std::uint32_t currentAddr = vaddr + (i * mem::kPageSize);
        
        //shift to get the page # and multiply by the size
        std::uint32_t offset = (currentAddr >> 14) * 4;
        
        //calculate the destination address
        std::uint32_t destAddr = offset + page_table_base;
        
        //or the current address with the present and writable bits
        std::uint32_t presAndwrit = page_frames.at(i) | mem::kPTE_PresentMask | mem::kPTE_WritableMask;
        
        //put the physical address stuff into the page table
        mem->movb(destAddr, &presAndwrit, sizeof(presAndwrit));
    }
    
     //return the number of pages successfully allocated
    return didAlloc ? count : 0;
}

void PageTableManager::setWritable(mem::PMCB *pmcb, std::uint32_t vaddr, int count, bool status) {
    for (int i = 0; i < count; i++) {
        
        std::uint32_t currentAddr = vaddr + (i * mem::kPageSize);

        //shift to get the page # and multiply by the size
        std::uint32_t offset = (currentAddr >> 14) * 4;

        //calculate the destination address
        std::uint32_t destAddr = offset + pmcb->page_table_base;
        
        std::uint32_t entry;
        mem->movb(&entry, destAddr, sizeof(entry));

        //Pull out present bit
        std::uint32_t presentBit = (entry & kPTE_PresentMask) >> 1;

        if (presentBit == 1) {
            entry ^= (-status ^ entry) & 1UL;

            mem->movb(destAddr, &entry, sizeof(entry));
        }
    }
}

void PageTableManager::releaseAll(mem::PMCB *pmcb) {
    std::vector<mem::Addr> addresses;
    //TODO: We will have to generate a list of alloc'd addresses
    // from the page table, but we don't have to update the page table on dealloc
    // maybe we need to know how many pages have been allocated?
    fa->Release(addresses.size(), addresses);
}

