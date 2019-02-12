/* 
 * Interface to Virtual Memory (Memory Management Unit)
 * 
 * File:   MMU.h
 * Author: Mike Goss <mikegoss@cs.du.edu>
 */

#ifndef MEM_MMU_H
#define MEM_MMU_H

#include "Exceptions.h"
#include "PageTable.h"
#include "PMCB.h"
#include "TLB.h"

#include <memory>

namespace mem {

class MMU {
public:
/**
   * Constructor (TLB enabled)
   * 
   * MMU is initialized with virtual memory disabled. 
   * Set the PMCB and call enter_virtual_mode to enable virtual memory.
   * 
   * @param frame_count_ number of page frames to allocate in physical memory
   * @param tlb_size_ number of entries in TLB (must be > 0)
   * @throws std::bad_alloc if insufficient memory
   */
  MMU(Addr frame_count_, size_t tlb_size);
  
  /**
   * Constructor (TLB disabled)
   * 
   * MMU is initialized with virtual memory disabled (pmcb is 0). 
   * Set the PMCB non-zero to enable it. No TLB is used with MMU.
   * 
   * @param frame_count_ number of page frames to allocate in physical memory
   * @throws std::bad_alloc if insufficient memory
   */
  MMU(Addr frame_count_);
  
  ~MMU() { }
  
  MMU(const MMU &other) = delete;  // no copy constructor
  MMU(MMU &&other) = delete;       // no move constructor
  MMU operator=(const MMU &other) = delete;  // no copy assign
  MMU operator=(MMU &&other) = delete;       // no move assign

  /**
   * get_frame_count - return number of page frames allocated
   * 
   * @return number of page frames in physical memory 
   */
  Addr get_frame_count() const { return frame_count; }
  
  /**
   * movb - get a single byte from the specified virtual address
   * 
   * @param dest - destination byte
   * @param vaddress - address of data in memory
   * @return true if success, false if operation aborted by fault handler
   */
  bool movb(void *dest, Addr vaddress);
  
  /**
   * movb - copy a range of bytes to caller buffer
   * 
   * @param dest where to copy to
   * @param address source virtual address
   * @param count number of bytes to copy
   * @return true if success, false if operation aborted by fault handler
   */
  bool movb(void *dest, Addr vaddress, Addr count);

  /**
   * movb - store a single byte to the specified virtual address
   * 
   * @param vaddress - address of data in memory
   * @param src - pointer to data to store at address
   * @return true if success, false if operation aborted by fault handler
   */
  bool movb(Addr vaddress, void *src);

  /**
   * movb - copy a range of bytes into physical memory
   * 
   * @param vaddress virtual address destination
   * @param count number of bytes to copy
   * @param src source buffer
   * @return true if success, false if operation aborted by fault handler
   */
  bool movb(Addr vaddress, void *src, Addr count);
  
  /**
   * enter_virtual_mode - set kernel mode PMCB and put the MMU into virtual 
   *   mode. The system will save the specified PMCB as the kernel mode PMCB
   *   and set it as the active PMCB. The kernel mode PMCB may not be changed
   *   once virtual mode is entered.
   * 
   * @param kernel_pmcb PMCB to use in kernel mode
   * @throws InvalidMMUOperationException if already in virtual
   *         mode.
   */
  void enter_virtual_mode(const PMCB &kernel_mode_pmcb);
  
  /**
   * set_user_PMCB - set the Processor Memory Control Block to be used by the 
   *   MMU in user mode, and switch the MMU to user mode
   * 
   * If the MMU is in physical memory mode, this call has no effect.
   * 
   * @param new_pmcb - start using this PMCB
   * @throws InvalidMMUOperationException if new_pmcb is invalid
   */
  void set_user_PMCB(const PMCB &new_pmcb);
  
  /**
   * get_user_PMCB - get a copy of the current user mode PMCB contents
   * 
   * @param cur_pmcb location to store PMCB contents
   */
  void get_user_PMCB(PMCB &cur_pmcb) const { cur_pmcb = user_pmcb; }
  
  /**
   * set_kernel_PMCB - switch the MMU to kernel mode, using the PMCB
   *   set by the enter_virtual_mode function.
   * 
   * @return a copy of the PMCB in use before this call.
   */
  PMCB set_kernel_PMCB(void);
  
  /**
   * get_kernel_PMCB - get a copy of the current kernel mode PMCB contents
   * 
   * @param cur_pmcb location to store PMCB contents
   */
  void get_kernel_PMCB(PMCB &cur_pmcb) const { cur_pmcb = kernel_pmcb; }
  
  /**
   * ToPhysical - convert virtual address to physical address.
   * 
   * If virtual mode is enabled, map a virtual address to a physical address. 
   * If the TLB is enabled and the page containing the virtual address is in 
   * the TLB, the page table is not consulted. Otherwise, the address is mapped
   * using the page table, and the mapping is cached in the TLB (if enabled).
   * 
   * If virtual mode is disabled, paddress is set to vaddress. The TLB is
   * unused and unchanged in this case.
   * 
   * @param vaddress virtual address to map
   * @param paddress returns corresponding physical address (undefined if not mapped)
   * @param write_op true if mapping for a write operation
   * @throws InvalidMMUOperationException if bad page table or other errors
   * @return true if success, false if operation aborted by fault handler
   */
  bool ToPhysical(Addr vaddress, Addr &paddress, bool write_op);
  
  /**
   * get_byte_count - return total number of bytes transferred so far 
   *   to/from physical memory.
   * 
   * @return count of bytes transferred.
   */
  uint64_t get_byte_count() const { return phys_mem.get_byte_count(); }
  
  /**
   * isTLBEnabled - query whether MMU has TLB enabled
   * 
   * @return true if TLB enabled, false otherwise
   */
  bool isTLBEnabled() const { return tlb.get() != nullptr; }
  
  /**
   * FlushTLB - flush the TLB (ignored if TLB disabled)
   */
  void FlushTLB() { if (tlb) tlb->Flush(); }
  
  /**
   * get_TLBStats - get TLB statistics
   * 
   * @param stats statistics from TLB
   * @throws InvalidMMUOperationException if TLB not enabled
   */
  void get_TLBStats(TLB::TLBStats &stats);

  /**
   * FaultHandler - abstract base class for fault handler
   * 
   * To define a fault handler, create a derived class, supplying the Run
   * function. You may also include any other private data or other functions.
   * 
   * When a fault occurs, the Run method will be called with a const reference
   * to the user mode PMCB. The kernel mode PMCB is set before calling the Run
   * method, and the user mode PMCB is restored on exit.
   */
   class FaultHandler {
   public:
     virtual bool Run(const PMCB &pmcb) = 0;  // derived class must override
   protected:
     FaultHandler (){}; // protected - only a derived class object can be created
   };

  /**
   * SetPageFaultHandler - set the fault handler for page faults.
   *   The handler returns true to resume the operation which caused
   *   the page fault, false to abort the operation.
   * 
   * @param handler function called when page fault occurs
   */
  void SetPageFaultHandler(const std::shared_ptr<FaultHandler> &handler) {
    page_fault_handler = handler;
  }
  
  /**
   * SetWritePermissionFaultHandler - set the function to be called when a
   *   write permission fault occurs. The handler is called with a read-only 
   *   reference to the PMCB. The handler returns true to resume the operation  
   *   which caused the page fault, false to abort the operation. The user_data 
   *   pointer can point to any arbitrary data supplied by the caller.
   * 
   * @param handler function called when page fault occurs
   */
  void SetWritePermissionFaultHandler(const std::shared_ptr<FaultHandler> &handler) {
    write_permission_fault_handler = handler;
  }
  
private:
  Addr frame_count;  // number of frames allocated in physical memory
  PhysicalMemory phys_mem;
  bool virtual_mode;  // true if in virtual mode
  PMCB *pmcb;         // current MMU control information
  PMCB kernel_pmcb;   // kernel mode PMCB
  PMCB user_pmcb;     // user mode PMCB
  
  // TLB (null if TLB disabled)
  std::unique_ptr<TLB> tlb;
  
  // Fault handler data
  bool fault_handler_active;   // true while fault handler is running
  std::shared_ptr<FaultHandler> page_fault_handler;
  std::shared_ptr<FaultHandler> write_permission_fault_handler;
  
  /**
   * InitMemoryOperation - setup memory operation in PMCB
   * 
   * @param op - READ_OP or WRITE_OP
   * @param vaddress virtual address
   * @param count number of bytes
   * @param user_buffer pointer to caller buffer (must be at least count bytes)
   */
  void InitMemoryOperation(PMCB::PMCB_op op, 
                           Addr vaddress, 
                           Addr count, 
                           uint8_t *user_buffer);
  
  /**
   * Execute - execute a (possibly partially complete) operation using the
   *           current PMCB contents.  The PMCB should contain the state of the
   *           operation as expected stored by MMU.  Executing when no
   *           partially completed operation is pending 
   *           (PMCB.operation_state == NONE) will result in an 
   *           InvalidMMUOperationException.
   * 
   * @return true if success, false if operation aborted by fault handler
   */
  bool Execute(void);
};

}  // namespace mem

#endif /* MEM_MMU_H */

