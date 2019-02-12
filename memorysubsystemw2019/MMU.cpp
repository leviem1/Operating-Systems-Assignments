/* 
 * Interface to Virtual Memory (Memory Management Unit)
 * 
 * File:   MMU.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 */

#include "MMU.h"

#include "Exceptions.h"

namespace {
 
/**
 * DefaultHandler - throws exception if not replaced by user fault handler
 */
class DefaultHandler : public mem::MMU::FaultHandler {
public:
  DefaultHandler() {}
  virtual bool Run(const mem::PMCB &pmcb) {  // subclass must override
    throw mem::InvalidMMUOperationException("Missing FaultHandler");
  };
};
  
}  // namespace

namespace mem {

MMU::MMU(Addr frame_count_, size_t tlb_size)
: frame_count(frame_count_),
  phys_mem(frame_count_ * kPageSize),
  pmcb(&kernel_pmcb),
  tlb(std::make_unique<TLB>(tlb_size)),
  virtual_mode(false),
  fault_handler_active(false),
  page_fault_handler(std::make_shared<DefaultHandler>()),
  write_permission_fault_handler(std::make_shared<DefaultHandler>())
{
}

MMU::MMU(Addr frame_count_) 
: frame_count(frame_count_), 
  phys_mem(frame_count_ * kPageSize),
  pmcb(&kernel_pmcb),
  tlb(nullptr),
  virtual_mode(false),
  fault_handler_active(false),
  page_fault_handler(std::make_shared<DefaultHandler>()),
  write_permission_fault_handler(std::make_shared<DefaultHandler>())
{
}
  
void MMU::InitMemoryOperation(PMCB::PMCB_op op, 
                              Addr vaddress, 
                              Addr count, 
                              uint8_t* user_buffer) {
  pmcb->operation_state = op;
  pmcb->next_vaddress = vaddress;
  pmcb->remaining_count = count;
  pmcb->user_buffer = user_buffer;
}

bool MMU::ToPhysical(Addr vaddress, Addr& paddress, bool write_op) {
  // If not in virtual memory mode, physical == virtual
  if (!virtual_mode) {
    paddress = vaddress;
    return true;
  }
  
  // If virtual address exceeds address space size, fail
  if ((vaddress & ~kVirtAddrMask) != 0) {
    throw InvalidMMUOperationException(
            "Virtual address exceeds address space size");
  }
  
  // If address translation cached in TLB, use it
  PageTableEntry pt_entry = 0;  // page table entry contents
  Addr pt_entry_addr = 0xFFFFFFFF;  // page table entry address
  bool from_tlb = false;  // true if translation from TLB
  
  if (tlb) {
    pt_entry = tlb->Lookup(vaddress);
    // Use TLB entry if page present. If this is a write and the modified
    // bit is not set in the cache, force use of page table so that 
    // the modified bit will be updated in the page table.
    from_tlb = (pt_entry & kPTE_PresentMask) != 0
            && (!write_op || ((pt_entry & kPTE_ModifiedMask) != 0));
  }

  if (!from_tlb) {
    // Get entry from page table
    Addr pt_index = (vaddress >> kPageSizeBits) & kPageTableIndexMask;
    pt_entry_addr = pmcb->page_table_base + pt_index * sizeof(PageTableEntry);
    phys_mem.movb32(&pt_entry, pt_entry_addr);
  }

  // Check for page present; if not, call page fault handler
  if ((pt_entry & kPTE_PresentMask) == 0) {
    PMCB *saved_pmcb = pmcb;
    pmcb = &kernel_pmcb;  // switch to kernel mode
    if (tlb) tlb->Flush();
    bool retry = page_fault_handler->Run(*saved_pmcb);

    // Restore saved state
    pmcb = saved_pmcb;
    if (tlb) tlb->Flush();
    
    // If aborted
    if (!retry) {
      return false;
    }
    
    // Re-read page table entry and recheck state
    phys_mem.movb32(&pt_entry, pt_entry_addr);
    if ((pt_entry & kPTE_PresentMask) == 0) {  // if still missing
      throw InvalidMMUOperationException(
              "Page fault handler returned true but page not present");
    }
  }
  
  // If write operation and page not writable, call fault hander
  if (write_op && (pt_entry & kPTE_WritableMask) == 0) {
    PMCB &saved_pmcb = *pmcb;
    pmcb = &kernel_pmcb;  // switch to kernel mode
    bool retry = write_permission_fault_handler->Run(saved_pmcb);
    pmcb = &saved_pmcb;
    if (!retry) {
      return false;
    }
    if ((pt_entry & kPTE_WritableMask) == 0) { // if still protected
      throw InvalidMMUOperationException(
            "Write permission fault handler returned true but page still protected");
    }
  }
  
  // If address not from TLB, set accessed and (optionally) modified flags 
  // in 2nd level table, then update the TLB
  if (!from_tlb) {
    PageTableEntry new_pt_entry = pt_entry | kPTE_AccessedMask
            | (write_op ? kPTE_ModifiedMask : 0);

    // If changed, write back to page table
    if (new_pt_entry != pt_entry) {
      pt_entry = new_pt_entry;
      phys_mem.movb(pt_entry_addr, reinterpret_cast<uint8_t*>(&pt_entry),
                    sizeof(PageTableEntry));
    }
    
    // Update TLB
    if (tlb) {
      tlb->Cache(vaddress, pt_entry);
    }
  }
  
  // Page is mapped, return physical
  paddress = (pt_entry & kPTE_FrameMask) | (vaddress & kPageOffsetMask);
  
  return true;
}

bool MMU::Execute() {
  if (pmcb->operation_state == PMCB::NONE) return true;
  
  if (pmcb->operation_state != PMCB::READ_OP && pmcb->operation_state != PMCB::WRITE_OP) {
    throw InvalidMMUOperationException("PMCB Error: operation is invalid");
  }
  
  while (pmcb->remaining_count > 0) {
    // Check if next page is mapped and has correct write permission
    Addr next_paddress;
    if (!ToPhysical(pmcb->next_vaddress, next_paddress, 
               pmcb->operation_state == PMCB::WRITE_OP)) {
      return false;
    }
    
    // Determine remaining count within current page
    Addr count_in_page = std::min(pmcb->remaining_count,
                                  kPageSize - (pmcb->next_vaddress & kPageOffsetMask));
    
    // Transfer bytes
    if (pmcb->operation_state == PMCB::READ_OP) {
      phys_mem.movb(pmcb->user_buffer, next_paddress, count_in_page);
    } else {  // write
      phys_mem.movb(next_paddress, pmcb->user_buffer, count_in_page);
    }
    
    // Advance state of transfer
    pmcb->next_vaddress += count_in_page;
    pmcb->user_buffer += count_in_page;
    pmcb->remaining_count -= count_in_page;
  }
  
  return true;
}

bool MMU::movb(void *dest, Addr vaddress) {
  InitMemoryOperation(PMCB::READ_OP, vaddress, 1, 
          reinterpret_cast<uint8_t*>(dest));
  return Execute();
}

bool MMU::movb(void *dest, Addr vaddress, Addr count) {
  InitMemoryOperation(PMCB::READ_OP, vaddress, count, 
          reinterpret_cast<uint8_t*>(dest));
  return Execute();
}

bool MMU::movb(Addr vaddress, void *src) {
  InitMemoryOperation(PMCB::WRITE_OP, vaddress, 1, 
          reinterpret_cast<uint8_t*>(src));
  return Execute();  
}

bool MMU::movb(Addr vaddress, void *src, Addr count) {
  InitMemoryOperation(PMCB::WRITE_OP, vaddress, count, 
          reinterpret_cast<uint8_t*>(src));
  return Execute();
}

void MMU::enter_virtual_mode(const PMCB &kernel_mode_pmcb) {
  if (virtual_mode) {
    throw InvalidMMUOperationException(
            "enter_virtual_mode called when already in virtual mode");
  } else {
    virtual_mode = true;
    kernel_pmcb = kernel_mode_pmcb;
    pmcb = &kernel_pmcb;
  }
}

void MMU::set_user_PMCB(const PMCB &new_pmcb) {
  if (fault_handler_active) {
    throw InvalidMMUOperationException("set_user_PMCB invalid in fault handler");
  }
  if ((new_pmcb.page_table_base & kPageOffsetMask) != 0) {
    throw InvalidMMUOperationException("page_table_base must be on a page boundary");
  }
  user_pmcb = new_pmcb;
  user_pmcb.operation_state = PMCB::NONE;
  pmcb = &user_pmcb;
}

void MMU::get_TLBStats(TLB::TLBStats& stats) {
  if (tlb.get() != nullptr) {
    tlb->get_stats(stats);
  } else {
    throw InvalidMMUOperationException("TLB is not enabled, stats not available");
  }
}

PMCB MMU::set_kernel_PMCB(void) {
  PMCB *prev_pmcb = pmcb;
  pmcb = &kernel_pmcb;
  return *prev_pmcb;
}

}  // namespace mem