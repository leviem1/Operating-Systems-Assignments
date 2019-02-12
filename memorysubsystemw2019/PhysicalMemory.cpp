/* Interface to physical memory
 * 
 * File:   PhysicalMemory.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 * 
 * Created on June 29, 2017, 11:45 AM
 */

#include "PhysicalMemory.h"

#include "Exceptions.h"
#include <cstring>

namespace mem {

std::vector<uint8_t> PhysicalMemory::mem_data;
    
PhysicalMemory::PhysicalMemory(Addr size) {
    if (size == 0)
      throw PhysicalMemoryZeroSizeException();
    if (!mem_data.empty()) 
      throw PhysicalMemoryDuplicateException();
    mem_data.reserve(size);
    mem_data.resize(size);
    byte_count = 0;
}

void PhysicalMemory::ValidateAddressRange(Addr address, Addr count) const {
  // Invalid if either end address is past end of memory, or if wrap-around
  // or 0 size block
  Addr end = address + count;
  if (end > mem_data.size() || end <= address) {
    throw PhysicalMemoryBoundsException(address);
  }
}

void PhysicalMemory::movb(uint8_t *dest, Addr address) {
  ValidateAddressRange(address, 1);
  ++byte_count;
  *dest = mem_data[address];
}

void PhysicalMemory::movb(uint8_t *dest, Addr address, Addr count) {
  ValidateAddressRange(address, count);
  byte_count += count;
  memcpy(dest, &mem_data[address], count);
}

void PhysicalMemory::movb(Addr address, uint8_t *data) {
  ValidateAddressRange(address, 1);
  ++byte_count;
  mem_data[address] = *data;
}

void PhysicalMemory::movb(Addr address, const uint8_t *src, Addr count) {
  ValidateAddressRange(address, count);
  byte_count += count;
  memcpy(&mem_data[address], src, count);
}

} // namespace mem