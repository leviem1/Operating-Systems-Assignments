/* 
 * File:   MMUTests
 * Author: Mike Goss <mikegoss@cs.du.edu>
 */

#include "MMU.h"
#include "Exceptions.h"

#include <gtest/gtest.h>
#include <cstring>

using namespace mem;

namespace {  // unnamed namespace for local functions

/**
 * RandBuf - initialize a buffer with random data. It doesn't need to be
 *   especially random, just different every time, so we use the (not very good)
 *   rand() function.
 * 
 * @param buf buffer to initialize
 * @param buf_len number of bytes to initialize
 */
void RandBuf(uint8_t *buf, Addr buf_len) {
  for (Addr i = 0; i < buf_len; ++i) {
    buf[i] = 0xFF & rand();
  }
}

}  // namespace

class MMUTests : public testing::Test {
protected:

  void SetUp() override {
    // Setup ...
  }

  void TearDown() override {
    // Teardown ...
  }
  
  /**
   * PageFaultTestHandler - page fault handler for testing
   * 
   * This handler always returns false to abort the operation.
 */
  class PageFaultTestHandler : public mem::MMU::FaultHandler {
  public:
    PageFaultTestHandler() : fault_count(0) {}
    
    /**
     * Run - handle fault
     * 
     * Increment fault count, save pmcb.
     * 
     * @param pmcb
     * @return 
     */
    virtual bool Run(const mem::PMCB &pmcb) {
      last_pmcb = pmcb;
      ++fault_count;
      return false;
    }
    
    int get_fault_count() const { return fault_count; }
    void reset_fault_count() { fault_count = 0; }
    void get_last_pmcb(PMCB &pmcb) const { pmcb = last_pmcb; }
  private:
    // Count of number of times handler was called
    int fault_count;
    
    // PMCB from last fault handled
    mem::PMCB last_pmcb;
  };
  
  /**
   * WritePermissionFaultTestHandler - page fault handler for testing
   * 
   * This handler always returns false to abort the operation.
   */
  class WritePermissionFaultTestHandler : public mem::MMU::FaultHandler {
  public:
    WritePermissionFaultTestHandler() : fault_count(0) {}
    
    /**
     * Run - handle fault
     * 
     * Increment fault count, save pmcb.
     * 
     * @param pmcb
     * @return 
     */
    virtual bool Run(const mem::PMCB &pmcb) {
      last_pmcb = pmcb;
      ++fault_count;
      return false;
    }
    
    int get_fault_count() const { return fault_count; }
    void reset_fault_count() { fault_count = 0; }
  private:
    // Count of number of times handler was called
    int fault_count;
    
    // PMCB from last fault handled
    mem::PMCB last_pmcb;
  };
  
  /**
   * BuildKernelPageTable - build a page table that can access all of physical
   * memory 1:1 with virtual memory, as would be used by the OS in kernel mode.
   * 
   * @param vm MMU to use (must not be in virtual mode yet)
   * @param pt_addr address at which to build page table
   */
  void BuildKernelPageTable(MMU &vm, Addr pt_addr) {
    PageTable kernel_page_table;  // local copy of page table to build, initialized to 0
    Addr num_pages = vm.get_frame_count();  // size of physical memory
    
    // Build page table entries
    for (Addr i = 0; i < num_pages; ++i) {
      kernel_page_table.at(i) = 
              (i << kPageSizeBits) | kPTE_PresentMask | kPTE_WritableMask;
    }
    
    // Write page table to memory
    vm.movb(pt_addr, &kernel_page_table, kPageTableSizeBytes);
  }

  void VMSinglePageTests(MMU &vm) {
    ASSERT_LE(32, vm.get_frame_count());  // make sure there are enough pages
    const Addr page_count = vm.get_frame_count(); // number of physical memory pages
    
    // Pick some page frame addresses to use for tests (in an actual OS, these 
    // page frame numbers would come from a memory allocator)
    const Addr kKernelPageTableBase = 1 * kPageSize;  // base of kernel page table
    const Addr kPageTableBase = 19 * kPageSize; // base of user mode page table

    const Addr kVAddrStart = 0x123 * kPageSize; // starting virtual address of page
    const Addr kPhysStart = 30 * kPageSize; // starting physical address of page
    
    // Set up kernel page table and enter virtual mode
    BuildKernelPageTable(vm, kKernelPageTableBase);
    PMCB kernel_pmcb(kKernelPageTableBase);
    vm.enter_virtual_mode(kernel_pmcb);

    // Build user mode page table and write to memory
    PageTable page_table;
    Addr pt_offset = (kVAddrStart >> kPageSizeBits) & kPageTableIndexMask;
    page_table.at(pt_offset) = kPhysStart | kPTE_PresentMask | kPTE_WritableMask;
    vm.movb(kPageTableBase, &page_table, kPageTableSizeBytes);
  
    // Switch to user mode page table
    PMCB vm_pmcb(kPageTableBase); // load to start virtual mode
    vm.set_user_PMCB(vm_pmcb);
    vm.FlushTLB();

    // Generate random data for test
    uint8_t random_bytes[kPageSize];
    RandBuf(random_bytes, kPageSize);

    // Write data one byte at a time, read back, and check
    for(Addr i = 0; i < kPageSize; ++i) {
      vm.movb(kVAddrStart + i, &random_bytes[i]);
    }
    for(Addr i = 0; i < kPageSize; ++i) {
      uint8_t byte_read;
      vm.movb(&byte_read, kVAddrStart + i);
      ASSERT_EQ(random_bytes[i], byte_read);
    }

    // Write full page, read back, and check
    RandBuf(random_bytes, kPageSize); // generate new random data
    uint8_t read_back[kPageSize];
    vm.movb(kVAddrStart, random_bytes, kPageSize);
    vm.movb(read_back, kVAddrStart, kPageSize);
    ASSERT_EQ(0, memcmp(random_bytes, read_back, kPageSize));

    // Check that present bit is observed
    std::shared_ptr<PageFaultTestHandler> pf_handler(
        std::make_shared<PageFaultTestHandler>());  // define a page fault handler
    vm.SetPageFaultHandler(pf_handler);

    uint8_t tmp;
    vm.movb(&tmp, kVAddrStart + kPageSize);
    ASSERT_EQ(1, pf_handler->get_fault_count());  // should have a page fault
    
    vm.movb(&tmp, kVAddrStart - 1);
    ASSERT_EQ(2, pf_handler->get_fault_count());  // should have a page fault
    pf_handler->reset_fault_count();

    // Check that writable bit is observed
    vm.set_kernel_PMCB();  // switch to kernel mode
    vm.FlushTLB();
    page_table[pt_offset] = kPhysStart | kPTE_PresentMask; // present but not writable
    vm.movb(kPageTableBase, &page_table, kPageTableSizeBytes);
    vm.set_user_PMCB(vm_pmcb); // back to user mode
    vm.FlushTLB();

    memset(read_back, 0, sizeof(read_back));
    vm.movb(read_back, kVAddrStart, kPageSize); // make sure we can read the page
    ASSERT_EQ(0, memcmp(random_bytes, read_back, kPageSize));

    // Set up write fault handler
    std::shared_ptr<WritePermissionFaultTestHandler> wpf_handler(
          std::make_shared<WritePermissionFaultTestHandler>());
    vm.SetWritePermissionFaultHandler(wpf_handler);
    
    // Test write fault handling
    tmp = 0x55;
    vm.movb(kVAddrStart, &tmp); // try writing - should fail
    ASSERT_EQ(1, wpf_handler->get_fault_count());
    wpf_handler->reset_fault_count();
    
    // Check that writable bit not set by page fault
    vm.set_kernel_PMCB(); // enter kernel mode
    vm.FlushTLB();
    vm.movb(&page_table, kPageTableBase, kPageTableSizeBytes); // read page table
    ASSERT_EQ(0, page_table[pt_offset] & kPTE_ModifiedMask);

    // Make page present and writable, write, and check modified bit
    page_table[pt_offset] = kPhysStart | kPTE_PresentMask | kPTE_WritableMask;
    vm.movb(kPageTableBase, &page_table, kPageTableSizeBytes);
    vm.set_user_PMCB(vm_pmcb); // back to user mode
    vm.FlushTLB();

    uint8_t write_byte = 0x55;
    vm.movb(kVAddrStart, &write_byte);
    vm.set_kernel_PMCB(); // back to kernel
    vm.FlushTLB();
    vm.movb(&page_table, kPageTableBase, kPageTableSizeBytes); // read page table
    ASSERT_NE(0, page_table[pt_offset] & kPTE_ModifiedMask);
    
    // Check that no further faults occurred
    ASSERT_EQ(0, pf_handler->get_fault_count());
    ASSERT_EQ(0, wpf_handler->get_fault_count());
  }

    
  /**
   * RemapPageFaultTestHandler - page fault handler for testing
   * 
   * This handler will map the faulted page and return true for a retry.
   */
  class RemapPageFaultTestHandler : public mem::MMU::FaultHandler {
  public:
    /**
     * Constructor - save available page address
     * @param avail_page
     */
    RemapPageFaultTestHandler(MMU &vm_, Addr avail_page_) 
    : fault_count(0), vm(vm_), avail_page(avail_page_) {}
    
    /**
     * Run - handle fault
     * 
     * If available page, remap it at fault address and resume, otherwise
     * abort.
     * 
     * @param pmcb
     * @return 
     */
    virtual bool Run(const mem::PMCB &pmcb) {
      ++fault_count;
      last_pmcb = pmcb;
      if (avail_page) {
        // Compute address of page table entry
        Addr pt_entry_addr = pmcb.page_table_base + 
                (pmcb.next_vaddress >> kPageSizeBits) * sizeof(PageTableEntry);
        
        // Create entry for faulted page and write to page table
        PageTableEntry pt_entry = avail_page | kPTE_PresentMask | kPTE_WritableMask;
        vm.movb(pt_entry_addr, &pt_entry, sizeof(PageTableEntry));
        avail_page = 0;  // mark as used
        return true;  // request retry
      } else {
        return false;  // abort operation
      }
    }
    
    int get_fault_count() const { return fault_count; }
    void reset_fault_count() { fault_count = 0; }
    void get_last_pmcb(PMCB &pmcb) const { pmcb = last_pmcb; }
  private:
    // Count of number of times handler was called
    int fault_count;
    
    // Virtual memory
    MMU &vm;
    
    // Physical address of an available page to remap (0 if already used)
    Addr avail_page;
    
    // PMCB from last fault handled
    mem::PMCB last_pmcb;
  };

  
  void VMMultiPageTests(MMU &vm) {
    ASSERT_LE(32, vm.get_frame_count());        // make sure we have enough pages
    const Addr kKernelPageTableBase = 30 * kPageSize;  // base of kernel page table
    const Addr kVAddrStart = (kPageTableEntries - 5) * kPageSize;
                                      // starting virtual address of user pages
    const Addr kVPageCount = 3;  // number of user pages
    const Addr kByteCount = kVPageCount * kPageSize;  // total bytes in user pages
  
    // Starting physical addresses of user pages
    const Addr kPhysStart[]
            = {29 * kPageSize, 6 * kPageSize, 31 * kPageSize};
    const Addr kPageTableBase = 19 * kPageSize; // base of user page table
    
    // Set up kernel page table and enter virtual mode
    BuildKernelPageTable(vm, kKernelPageTableBase);
    PMCB kernel_pmcb(kKernelPageTableBase);
    vm.enter_virtual_mode(kernel_pmcb);

    // Build user page table and write to memory
    PageTable page_table; // user page table
    Addr pt_index = (kVAddrStart >> kPageSizeBits) & kPageTableIndexMask;
    for (Addr i = 0; i < kVPageCount; ++i) {
      page_table.at(pt_index+i) = kPhysStart[i] | kPTE_PresentMask | kPTE_WritableMask;
    }
    vm.movb(kPageTableBase, &page_table, kPageTableSizeBytes);

    // Switch to user mode
    PMCB vm_pmcb(kPageTableBase); // load to set user page table
    vm.set_user_PMCB(vm_pmcb);
    vm.FlushTLB();

    // Generate random data for test
    uint8_t random_bytes[kByteCount];
    RandBuf(random_bytes, kByteCount);

    // Write data one byte at a time, read back, and check
    for(Addr i = 0; i < kByteCount; ++i) {
      vm.movb(kVAddrStart + i, &random_bytes[i]);
    }
    for(Addr i = 0; i < kByteCount; ++i) {
      uint8_t byte_read;
      vm.movb(&byte_read, kVAddrStart + i);
      ASSERT_EQ(random_bytes[i], byte_read);
    }

    // Write all 3 pages in one write, read back, and compare
    RandBuf(random_bytes, kByteCount);
    uint8_t read_back[kByteCount];
    vm.movb(kVAddrStart, random_bytes, kByteCount);
    vm.movb(read_back, kVAddrStart, kByteCount);
    ASSERT_EQ(0, memcmp(random_bytes, read_back, kByteCount));

    // Unmap the 3rd page
    PageTableEntry pt_save = page_table[pt_index+2]; // save entry for third page
    page_table[pt_index+2] = 0;
    vm.set_kernel_PMCB();  // switch to kernel page table
    vm.FlushTLB();
    vm.movb(kPageTableBase, &page_table, kPageTableSizeBytes);
    vm.set_user_PMCB(vm_pmcb);  // return to user mode
    vm.FlushTLB();

    // Try a read and make sure it page faults
    std::shared_ptr<PageFaultTestHandler> pf_handler(
          std::make_shared<PageFaultTestHandler>());  // define a page fault handler
    vm.SetPageFaultHandler(pf_handler);

    memset(read_back, 0, kByteCount);
    vm.movb(read_back, kVAddrStart, kByteCount);
    ASSERT_EQ(1, pf_handler->get_fault_count());  // make sure fault occurred
    PMCB tmp_pmcb;
    pf_handler->get_last_pmcb(tmp_pmcb);
    ASSERT_EQ(kVAddrStart + 2 * kPageSize, tmp_pmcb.next_vaddress);
    ASSERT_EQ(kPageSize, tmp_pmcb.remaining_count);
    ASSERT_EQ(read_back + 2 * kPageSize, tmp_pmcb.user_buffer);

    // Check that 1st two pages were read, 3rd page wasn't
    ASSERT_EQ(0, memcmp(random_bytes, read_back, kPageSize * 2));
    ASSERT_NE(0, memcmp(random_bytes + kPageSize * 2, read_back + kPageSize * 2, kPageSize));

    // Set up page fault handler to remap the 3rd page during fault handling
    std::shared_ptr<RemapPageFaultTestHandler> rpf_handler(
          std::make_shared<RemapPageFaultTestHandler>(vm, kPhysStart[2]));
    vm.SetPageFaultHandler(rpf_handler);

    // Write the all 3 page again (3rd page should be remapped)
    vm.movb(kVAddrStart, random_bytes, kByteCount);
    ASSERT_EQ(1, rpf_handler->get_fault_count());
    PMCB last_pmcb;
    rpf_handler->get_last_pmcb(last_pmcb);
    ASSERT_EQ(kVAddrStart + kPageSize*2, last_pmcb.next_vaddress);
    
    // Read back all 3 pages (should be no fault)
    rpf_handler->reset_fault_count();
    memset(read_back, 0, kByteCount);
    vm.movb(read_back, kVAddrStart, kByteCount);
    ASSERT_EQ(0, rpf_handler->get_fault_count());
    ASSERT_EQ(0, memcmp(random_bytes, read_back, kByteCount));


//    PMCB partial_pmcb;
//    vm.get_user_PMCB(partial_pmcb); // save PMCB of read in progress
//
//    page_table[pt_index+2] = pt_save;
//    vm.set_kernel_PMCB();
//    vm.FlushTLB();
//    vm.movb(kPageTableBase, &page_table, kPageTableSizeBytes);
//
//    // Reload PMCB and finish read, check for correct completion
//    vm.set_user_PMCB(partial_pmcb);
//    vm.FlushTLB();
//    ASSERT_EQ(0, memcmp(random_bytes, read_back, kByteCount));
  }
};

/**
 * Test exceptions for zero size or duplicate memory
 */
TEST_F(MMUTests, ZeroDuplicate) {
  MMU vm1(8);
  MMU *vm2;
  EXPECT_THROW(vm2 = new MMU(4),
          mem::PhysicalMemoryDuplicateException);
  EXPECT_THROW(vm2 = new MMU(0),
          mem::PhysicalMemoryZeroSizeException);
}


/***********************************************************************
 * Run all the MMU tests with virtual memory disabled
 ***********************************************************************/

TEST_F(MMUTests, ConstructorAndSize) {
  // Allocate various sizes and check for correct returned size
  {
    const Addr kPageCount = 1;
    MMU vm(kPageCount);
    ASSERT_EQ(kPageCount, vm.get_frame_count());
    ASSERT_EQ(0, vm.get_byte_count());
  }
  {
    const Addr kPageCount = 16;
    MMU vm(kPageCount);
    ASSERT_EQ(kPageCount, vm.get_frame_count());
    ASSERT_EQ(0, vm.get_byte_count());
  }
  {
    const Addr kPageCount = 256;
    MMU vm(kPageCount);
    ASSERT_EQ(kPageCount, vm.get_frame_count());
    ASSERT_EQ(0, vm.get_byte_count());
  }
}

/**
 * Test put/get of single byte
 */
TEST_F(MMUTests, PutGetByte) {
  // Write then read every byte
  const Addr kPageCount = 2;
  MMU vm(kPageCount);
  uint64_t expected_byte_count = 0;
  uint8_t data_byte = 0;
  const Addr kSize = vm.get_frame_count() * kPageSize;
  
  // Initialize every byte to 0xFF
  uint8_t xff = 0xFF;
  for (Addr addr = 0; addr < kSize; ++addr) {
    vm.movb(addr, &xff);
    ASSERT_EQ(0xFF, xff);  // should be unchanged
    vm.movb(&data_byte, addr);
    ASSERT_EQ(0xFF, data_byte);
    expected_byte_count += 2;
    ASSERT_EQ(expected_byte_count, vm.get_byte_count());
  }
  
  // Write different values to every byte and check value
  uint8_t x55 = 0x55;
  for (Addr addr = 0; addr < kSize; ++addr) {
    vm.movb(addr, &x55);
    ASSERT_EQ(0x55, x55);  // should be unchanged
    vm.movb(&data_byte, addr);
    ASSERT_EQ(0x55, data_byte);
    uint8_t addr_hash = 0xFF & (addr ^ (addr >> 8));
    vm.movb(addr, &addr_hash);
    vm.movb(&data_byte, addr);
    ASSERT_EQ(addr_hash, data_byte);
    expected_byte_count += 4;
    ASSERT_EQ(expected_byte_count, vm.get_byte_count());
  }
}

/**
 * Test put/get of block of bytes
 */
TEST_F(MMUTests, PutGetBytes) {
  // Read/write blocks of bytes of various lengths
  const Addr kPageCount = 1;
  MMU vm(kPageCount);
  const Addr kMaxSize = kPageCount * kPageSize;
  uint8_t put_buf[kMaxSize+1];
  uint8_t get_buf[kMaxSize+1];
  memset(put_buf, 0, kMaxSize+1);  // clear put buffer
  uint64_t expected_byte_count = 0;
  
  // Try a selection of possible sizes
  Addr size_incr = kMaxSize / 29;
  for (Addr size = 1; size <= kMaxSize; size += size_incr) {
    put_buf[size] = 0x55;  // sentinel at end of put buffer
    // Try a selection of possible addresses
    Addr start_incr = std::min(kMaxSize / 31, size);
    for (Addr start = 0; start <= (kMaxSize - size); start += start_incr) {
      memset(get_buf, 0, size);  // clear
      get_buf[size] = 0xAA;  // sentinel at end of buffer to check overwrite
      RandBuf(put_buf, size);  // generate random data to write
      vm.movb(start, put_buf, size);  // write data to memory
      vm.movb(get_buf, start, size);  // read back data
      ASSERT_EQ(0, std::memcmp(put_buf, get_buf, size));  // compare
      ASSERT_EQ(0xAA, get_buf[size]);  // check sentinel unchanged
      expected_byte_count += 2 * size;
      ASSERT_EQ(expected_byte_count, vm.get_byte_count());
    }
  }
}

/**
 * Test invalid addresses
 */
TEST_F(MMUTests, AddressCheck) {
  const Addr kPageCount = 2;
  const Addr kSize = kPageCount * kPageSize;
  MMU vm(kPageCount);
  uint8_t data_byte;
  Addr expected_bytes = 0;  // expected number of bytes transferred
  
  // Test one past end
  {
    bool success = false;
    try {
      vm.movb(&data_byte, kSize);
    }
    catch (PhysicalMemoryBoundsException e) {
      ASSERT_STREQ("PhysicalMemoryBoundsException, block starting at 0x8000", 
                   e.what());
      success = true;  // success if exception caught
    }
    ASSERT_TRUE(success);
  }
  
  // Test one past end
  {
    bool success = false;
    uint8_t x1 = 1;
    try {
      vm.movb(kSize, &x1);
    }
    catch (PhysicalMemoryBoundsException e) {
      ASSERT_STREQ("PhysicalMemoryBoundsException, block starting at 0x8000", 
                   e.what());
      success = true;  // success if exception caught
    }
    ASSERT_TRUE(success);
  }
    
  // Test one past end (block)
  {
    bool success = false;
    uint8_t buf[8];
    try {
      expected_bytes += 7;
      vm.movb(buf, kSize-8+1, 8);
    }
    catch (PhysicalMemoryBoundsException e) {
      ASSERT_STREQ("PhysicalMemoryBoundsException, block starting at 0x8000", 
                   e.what());
      success = true;  // success if exception caught
    }
    ASSERT_TRUE(success);
  }
    
  // Test one past end (block)
  {
    bool success = false;
    uint8_t buf[8];
    try {
      expected_bytes += 7;
      vm.movb(kSize-8+1, buf, 8);
    }
    catch (PhysicalMemoryBoundsException e) {
      ASSERT_STREQ("PhysicalMemoryBoundsException, block starting at 0x8000", 
                   e.what());
      success = true;  // success if exception caught
    }
    ASSERT_TRUE(success);
  }

  // Test max possible address
  {
    bool success = false;
    Addr addr = 0xFFFFFFFF;  // largest possible
    ASSERT_EQ(4, sizeof(addr));      // test assumes 32 bit addresses
    try {
      vm.movb(&data_byte, addr);
    }
    catch (PhysicalMemoryBoundsException e) {
      ASSERT_STREQ("PhysicalMemoryBoundsException, block starting at 0xffffffff", 
                   e.what());
      success = true;  // success if exception caught
    }
    ASSERT_TRUE(success);
  }
    
  // Test wrap-around
  {
    bool success = false;
    Addr addr = 0xFFFFFFFD;  // nearly largest
    ASSERT_EQ(4, sizeof(addr));      // test assumes 64 bit addresses
    uint8_t buf[8];
    try {
      vm.movb(buf, addr, 4);
    }
    catch (PhysicalMemoryBoundsException e) {
      ASSERT_STREQ("PhysicalMemoryBoundsException, block starting at 0xfffffffd", 
                   e.what());
      success = true;  // success if exception caught
    }
    ASSERT_TRUE(success);
  }
  
  ASSERT_EQ(expected_bytes, vm.get_byte_count());  // only parts of reads/writes should succeed
}

/***********************************************************************
 * Run MMU tests with virtual memory enabled
 ***********************************************************************/

// Tests with just a single page mapped
TEST_F(MMUTests, SinglePage) {
  const Addr kPageCount = 32;  // number of physical memory pages
  // Run tests with TLB disabled
  MMU vm(kPageCount);
  ASSERT_FALSE(vm.isTLBEnabled());
  VMSinglePageTests(vm);
  TLB::TLBStats stats;
  ASSERT_THROW(vm.get_TLBStats(stats), InvalidMMUOperationException);
}

TEST_F(MMUTests, SinglePageTLB) {
  const Addr kPageCount = 32;  // number of physical memory pages
  // Run tests with TLB enabled
  MMU vm(kPageCount, kPageCount/4);
  ASSERT_TRUE(vm.isTLBEnabled());
  VMSinglePageTests(vm);

  // Check that TLB was used by making sure that stats are not zero
  TLB::TLBStats stats;
  vm.get_TLBStats(stats);
  ASSERT_NE(0, stats.total_hits);
  ASSERT_NE(0, stats.total_misses);
  ASSERT_NE(0, stats.total_max_size);
}

// Test with three pages scattered in physical memory
TEST_F(MMUTests, MultiPage) {
  const Addr kPageCount = 32;  // number of physical memory pages
  // Run tests with TLB disabled
  MMU vm(kPageCount);
  VMMultiPageTests(vm);
}

TEST_F(MMUTests, MultiPageTLB) {
  const Addr kPageCount = 32;  // number of physical memory pages
  // Run tests with TLB enabled
  MMU vm(kPageCount, kPageCount/4);
  VMMultiPageTests(vm);

  // Check that TLB was used by making sure that stats are not zero
  TLB::TLBStats stats;
  vm.get_TLBStats(stats);
  ASSERT_NE(0, stats.total_hits);
  ASSERT_NE(0, stats.total_misses);
  ASSERT_NE(0, stats.total_max_size);
}
