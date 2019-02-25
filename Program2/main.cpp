
/* 
 * File:   main.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on February 18, 2019, 1:14 PM
 */

#include <cstdlib>
#include <MMU.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "FrameAllocator.h"
#include "Process.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    //create the MMU that will be passed around to all things throughout
    mem::MMU mem (128);
    FrameAllocator allocator (128, mem);
    
    //check for the proper number of args
    if (argc != 2) {
        cerr << "Usage: Program2 file" << endl;
        
        return 1;
    }
    
    //store the filename for use throughout the read.
    string filename = argv[1];
    
    //create and open the file
    ifstream myFile(filename);
    if (!myFile.is_open() || !myFile.good()) {
        throw runtime_error{ "Error: failed to open file: " + filename };
    }
    
    //process the file
    else {

//---------------Build the kernel page table and add to memory--------------//
        
        mem::PageTable kernel_page_table;  // local copy of page table to build, initialized to 0
        mem::Addr num_pages = mem.get_frame_count();  // size of physical memory
        
        //use the allocator the allocate kernel
        //TODO:Figure out what to give it for the vector of 32 int's 
    
        // Build page table entries
        for (mem::Addr i = 0; i < num_pages; ++i) {
            kernel_page_table.at(i) = 
            (i << mem::kPageSizeBits) | mem::kPTE_PresentMask 
                    | mem::kPTE_WritableMask;
        }
    
        // Write page table to start of physical memory
        mem.movb(0, &kernel_page_table, mem::kPageTableSizeBytes);
        
        //build the pmcb needed to pass the kernel table
        mem::PMCB kernel_pmcb(0);
        
//----------------------Enter Virtual Mode------------------------------//
        mem.enter_virtual_mode(kernel_pmcb);
        
        
        
        
        
//------------------------Close the file-------------------------------//
        myFile.close();
    } 
    
    
    

    return 0;
}

