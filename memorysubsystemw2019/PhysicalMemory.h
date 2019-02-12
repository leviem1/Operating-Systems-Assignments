/* Interface to physical memory
 *  
 * File:   PhysicalMemory.h
 * Author: Mike Goss <mikegoss@cs.du.edu>
 *
 * Created on June 29, 2017, 11:45 AM
 */

#ifndef MEM_PHYSICALMEMORY_H
#define MEM_PHYSICALMEMORY_H

#include "MemoryDefs.h"

#include <cstddef>
#include <vector>

namespace mem {

class PhysicalMemory {
public:
  /**
   * Constructor
   * 
   * @param size number of bytes of memory to allocate (must be a multiple
   *             of 16)
   * @throws std::bad_alloc if insufficient memory
   */
  PhysicalMemory(Addr size);
  
  virtual ~PhysicalMemory() { mem_data.clear(); }
  
  PhysicalMemory(const PhysicalMemory &other) = delete;  // no copy constructor
  PhysicalMemory(PhysicalMemory &&other) = delete;       // no move constructor
  PhysicalMemory &operator=(const PhysicalMemory &other) = delete; // no assign
  PhysicalMemory &operator=(PhysicalMemory &&other) = delete;  // no move assign

  /**
   * size - return size of physical memory
   * 
   * @return number of bytes in physical memory 
   */
  Addr size() const { return mem_data.size(); }
  
  /**
   * movb - get a single byte from the specified address
   * 
   * @param dest - destination byte pointer
   * @param address - address of data in memory
   */
  void movb(uint8_t *dest, Addr address);
  
  /**
   * movb - copy a range of bytes to caller buffer
   * 
   * @param dest where to copy to
   * @param address source address
   * @param count number of bytes to copy
   */
  void movb(uint8_t *dest, Addr address, Addr count);
  
  /**
   * movb32 - get a 32 bit (4 byte) value
   * 
   * @param dest where to copy to
   * @param address source address
   */
  void movb32(uint32_t *dest, Addr address) {
    movb(reinterpret_cast<uint8_t*>(dest), address, 4);
  }

  /**
   * movb - store a single byte to the specified address
   * 
   * @param address - address of data in memory
   * @param data - data to store at address
   */
  void movb(Addr address, uint8_t *data);

  /**
   * movb - copy a range of bytes into physical memory
   * 
   * @param address destination in physical memory
   * @param count number of bytes to copy
   * @param src source buffer
   */
  void movb(Addr address, const uint8_t *src, Addr count);
  
  /**
   * ValidateAddressRange - check that address range is valid, throw
   *   PhysicalMemoryBoundsException if not.
   * @param address
   * @param count
   */
  void ValidateAddressRange(Addr address, Addr count) const;
  
  /**
   * get_byte_count - return total number of bytes transferred so far 
   *   to/from physical memory.
   * 
   * @return count of bytes transferred.
   */
  uint64_t get_byte_count() const { return byte_count; }
  
private:
  // Actual memory contents; ensure only a single PhysicalMemory instance
  // can exist by making this static.
  static std::vector<uint8_t> mem_data;
  
  // Define counter for number of bytes transferred.  Can be used as
  // pseudo-clock for ordering of cache entries.
  uint64_t byte_count;  // increments by one for every request
};

} // namespace mem

#endif /* MEM_PHYSICALMEMORY_H */

