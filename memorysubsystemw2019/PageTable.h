/* 
 * Definitions for page table
 * 
 * MMU uses a single-level page table.  Each entry is 32 bits. The lower 14
 * bits are reserved for flag bits. The upper bits contain the page frame
 * number.
 * 
 * A virtual address is 26 bits, stored in a 32 bit value. The address contains
 * the offset in the page in the lower 14 bits.  The next 12 bits contain the 
 * page table offset. The upper 6 bits must be 0.
 * 
 * File:   PageTable.h
 * Author: Mike Goss <mikegoss@cs.du.edu>
 */

#ifndef MEM_PAGETABLE_H
#define MEM_PAGETABLE_H

#include "MemoryDefs.h"

#include <array>

namespace mem {

// Define storage for a single page table entry
typedef Addr PageTableEntry;

// Page table size
const int  kPageTableSizeBits = kPageSizeBits - 2;  // page table is 1 page
const Addr kPageTableEntries = (1 << kPageTableSizeBits);
static_assert(kPageTableEntries == kPageSize / sizeof(PageTableEntry),
              "Page size and page table size mismatch");
const Addr kPageTableSizeBytes = kPageSize;
const Addr kPageTableIndexMask = kPageTableEntries - 1;

// Define size of virtual address space, and mask for virtual address bits
const Addr kVirtAddrSpaceSize = kPageTableEntries * kPageSize;
const Addr kVirtAddrMask = kVirtAddrSpaceSize - 1;

// Define bit masks and shifts for fields in page table entry

// The page frame number is stored in the upper bits
const uint32_t kPTE_FrameMask = kPageNumberMask;

// Bit masks for other flags
const uint32_t kPTE_Present = 1;            // page present in memory
const uint32_t kPTE_PresentMask = (1 << kPTE_Present);
const uint32_t kPTE_Writable = 0;           // page is writable
const uint32_t kPTE_WritableMask = (1 << kPTE_Writable);
const uint32_t kPTE_Accessed = 2;           // set when page is accessed
const uint32_t kPTE_AccessedMask = (1 << kPTE_Accessed);
const uint32_t kPTE_Modified = 3;           // set when page is modified
const uint32_t kPTE_ModifiedMask = (1 << kPTE_Modified);

// Define type for a page table as a derived class from std::array.
// The page table is initialized to zero.
class PageTable : public std::array<PageTableEntry, kPageTableEntries> {
public:
  PageTable() {
    fill(0);
  }
};

}  // namespace mem

#endif /* MEM_PAGETABLE_H */

