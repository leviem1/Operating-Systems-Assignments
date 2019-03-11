/* 
 * File:   Process.h
 * Authors: Elisabeth Bristol and Levi Muniz
 *
 * Created on February 12, 2019, 4:44 PM
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <MMU.h>
#include "PageTableManager.h"

class WriteFaultHandler: public mem::MMU::FaultHandler {
public:
    WriteFaultHandler() = default;

    //explicit deletes
    WriteFaultHandler(const WriteFaultHandler& other) = delete;
    WriteFaultHandler(WriteFaultHandler&& other) = delete;
    WriteFaultHandler& operator=(const WriteFaultHandler& other) = delete;
    WriteFaultHandler& operator=(WriteFaultHandler&& other) = delete;

    ~WriteFaultHandler() = default;

    /**
     * Run - will print out the appropriate error message
     * @param pmcb  - the process pmcb where the fault was experienced
     * @return bool- will always return false - used for the errrors
     */
    virtual bool Run(const mem::PMCB &pmcb) {
        std::cout << "Write Permission Fault at address " << std::setfill('0')
                  << std::setw(7) << std::hex << pmcb.next_vaddress << '\n';

        return false;
    }
};

class PageFaultHandler: public mem::MMU::FaultHandler {
public:
    PageFaultHandler(PageTableManager &ptm, mem::MMU &mem);

    //explicit deletes
    PageFaultHandler(const PageFaultHandler& other) = delete;
    PageFaultHandler(PageFaultHandler&& other) = delete;
    PageFaultHandler& operator=(const PageFaultHandler& other) = delete;
    PageFaultHandler& operator=(PageFaultHandler&& other) = delete;

    ~PageFaultHandler() = default;

    /**
     * run - will print out the appropriate error message for 2 different
     * kinds of page faults.
     * @param pmcb - the pmcb of the process where the fault happened
     * @return
     */
    virtual bool Run(const mem::PMCB &pmcb);

    //TODO: Setter for quota

private:
    PageTableManager *ptm;
    mem::MMU *mem;
};

class Process {
public:
    /*Constructor for Process 
     * @param filename - name of the file we are reading from
     * @throws run time error if the file cannot be opened
     */
    Process(const std::string &filename, mem::MMU &mem, PageTableManager &ptm);

    //Process Destructor that closes the file and deals with memory leaks
    ~Process();

    //rule of 5 explicit deletes
    Process(const Process& other) = delete;
    Process(Process&& other) = delete;
    Process& operator=(const Process& other) = delete;
    Process& operator=(Process&& other) = delete;

    /* Exec function that acts as a parser for commands in the file
     * @return void
     */
    void Exec();

private:
    std::fstream *file;
    std::uint64_t line;
    mem::MMU *mem;
    PageTableManager *ptm;
    mem::PMCB *vm_pmcb;
    std::shared_ptr<WriteFaultHandler> *wf_handler;
    std::shared_ptr<PageFaultHandler> *pf_handler;

    /**
     * alloc - deals with the allocate command in a trace file.
     * allocates parts of memory using the frame allocator 
     * @param address - the starting virtual address to allocate, must be on a
     * frame border
     * @param pages - the number of pages to allocate
     * @returns - void
     */
    void alloc(int address, int pages);

    /*cmp - compares a number of bytes at two given addresses
     * @param address1 - first address in comparison
     * @param address2 - second address in comparison
     * @param count - number of bytes to be compared
     * @throws - standard error with message if the compared things don't match
     * @return void
     */
    void cmp(int address1, int address2, int count);

    /*set - takes a list of values and sets them in memory from an address
     * @param address - the starting spot in memory
     * @param v - a vector containing the list of values to add
     * @return - void
     */
    void set(int address, std::vector<std::uint8_t> &v);

    /* fill - puts count number of some value starting at an address
     * @param address - the starting address
     * @param value - the value we will use for the filling
     * @param count - the number of times we add that value
     * @return - void
     */
    void fill(int address, std::uint8_t value, int count);

    /*dup - copies a number of bytes starting at one location to new
     * @param src_address - the starting location for the copy
     * @param dest_address -  the destination of the copy
     * @param count - the number of bytes to be duplicated
     * @return - void
     */
    void dup(int src_address, int dest_address, int count);

    /* print - prints the status of memory in 16 byte intervals 
     * displays the address of each 16 byte chunks at the beginning of each row
     * @param address -  the starting point the print comes from
     * @param count - the number of bytes we are displaying
     * @return - void
     */
    void print(int address, int count);

    /**
     * Perm - deals with the perm command in a trace file. Will go through pages 
     * number of pages starting at the virtual address, address and 
     * set the write bit to whatever status is provided present bit set
     * @param address - starting virtual address to update write permissions
     * @param pages - number of pages to check to update write permissions
     * @param status - what to change the write permission to. True -> 1
     * False ->0
     * @return - void
     */
    void perm(int address, int pages, bool status);
};



#endif /* PROCESS_H */
