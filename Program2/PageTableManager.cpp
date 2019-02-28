

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

bool PageTableManager::allocate(std::uint32_t count, mem::PMCB *pmcb, uint32_t vaddr) {
    std::vector<uint32_t> page_frames;
    bool didAlloc = fa->Allocate(count, page_frames);

    for(int i = 0; i < page_frames.size(); i++){
        //calculate the offset that we need to add to the vaddr in the user page table
        std::uint32_t currentAddr = vaddr + (i * mem::kPageSize);
        
        //shift to get the page # and multiply by the size
        std::uint32_t offset = (currentAddr >> 14) * 4;
        
        //calculate the destination address
        std::uint32_t destAddr = offset + pmcb->page_table_base;
        
        //or the current address with the present and writeable bits
        std::uint32_t presAndwrit = page_frames.at(i) | mem::kPTE_PresentMask | mem::kPTE_WritableMask;
        
        //put the physical address stuff into the page table
        mem->movb(destAddr, &presAndwrit, sizeof(presAndwrit));
    }

    return didAlloc;
}

void PageTableManager::setWritable(mem::PMCB *pmcb, std::uint32_t vaddr, int count, bool status) {
    for (int i = 0; i < count; i++) {
        //calculate the offset that we need to add to the vaddr in the user page table
        std::uint32_t currentAddr = vaddr + (i * mem::kPageSize);

        //shift to get the page # and multiply by the size
        std::uint32_t offset = (currentAddr >> 14) * 4;

        //calculate the destination address
        std::uint32_t destAddr = offset + pmcb->page_table_base;

        //Pull out present bit
        std::uint32_t presentBit = (currentAddr & kPTE_PresentMask) >> 1;

        if (presentBit == 1) {
            currentAddr ^= (-status ^ currentAddr) & 1UL;

            mem->movb(destAddr, &currentAddr, sizeof(currentAddr));
        }
    }
}

