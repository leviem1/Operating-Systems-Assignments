
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
#include "FaultHandler.h"

using namespace std;
using namespace mem;

/*
 * 
 */
int main(int argc, char** argv) {
    //create the MMU that will be passed around to all things throughout
    mem::MMU mem (128);
    
    FrameAllocator allocator (128, mem);
    PageTableManager ptm(mem, allocator);
    
    //build the connections to the fault handlers
    std::shared_ptr<WriteFaultHandler> wpf_handler(
          std::make_shared<WriteFaultHandler>());
    mem.SetWritePermissionFaultHandler(wpf_handler);
    
    std::shared_ptr<PageFaultHandler> pf_handler(
          std::make_shared<PageFaultHandler>(ptm, mem));
    mem.SetPageFaultHandler(pf_handler);
    

    //check for the proper number of args
    if (argc != 2) {
        cerr << "Usage: Program3 file" << endl;
        
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

        
        Process p(argv[1], mem, ptm);
        p.Exec();
        
        
//------------------------Close the file-------------------------------//
        myFile.close();
    } 
    
    
    

    return 0;
}

