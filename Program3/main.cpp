
/* 
 * File:   main.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on February 18, 2019, 1:14 PM
 *
 *  /\\=//\-"""-.
 * / /6 6\ \     \
 *  =\_Y_/=  (_  ;{}
 *    /^//_/-/__/
 *    "" ""  """
 *    Team mascot
 */

#include <MMU.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "FrameAllocator.h"
#include "Process.h"

using namespace std;
using namespace mem;

/*
 * 
 */
int main(int argc, char** argv) {
    //create the MMU that will be passed around to all things throughout
    mem::MMU mem(128);


    FrameAllocator allocator(mem);
    PageTableManager ptm(mem, allocator);

    //check for the proper number of args
    if (argc <= 2) {
        cerr << "Usage: Program3 timeslice file1 [file2] ..." << endl;
        
        return 1;
    }

//---------------Build the kernel page table and add to memory--------------//
        
    mem::PageTable kernel_page_table;  // local copy of page table to build, initialized to 0
    mem::Addr num_pages = mem.get_frame_count();  // size of physical memory

   //allocate room for the kernel page table
    vector<mem::Addr> address;
    allocator.Allocate(1,address);

    // Build page table entries
    for (mem::Addr i = 0; i < num_pages; ++i) {
        kernel_page_table.at(i) =
        (i << mem::kPageSizeBits) | mem::kPTE_PresentMask
                | mem::kPTE_WritableMask;
    }

    // Write page table to start of physical memory
    mem.movb(address[0], &kernel_page_table, mem::kPageTableSizeBytes);

    //build the pmcb needed to pass the kernel table
    mem::PMCB kernel_pmcb(address[0]);
        
//----------------------Enter Virtual Mode/Execute File-----------------//

    mem.enter_virtual_mode(kernel_pmcb);
    
    std::vector<&Process> processes;
    
    std::vector<&int> test;
    
    for(int i = 0; i< 9;i++){
        test.push_back(i);
    }
    
    for(int i = 0; i< 9;i++){
        std::cout << test.at(i);
    }
    
   /**
    
    for (int i = 2; i < argc; i ++){
        Process p (argv[i], mem, ptm);
        processes.push_back(p);
    }
    */
    


    return 0;
}

