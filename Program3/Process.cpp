/* 
 * File:   Process.cpp
 * Authors: Elisabeth Bristol and Levi Muniz
 * 
 * Created on February 11, 2019, 4:44 PM
 */

#include "Process.h"


Process::Process(const std::string &filename, mem::MMU &mem, PageTableManager &ptm) {
    this->mem = &mem;
    this->ptm = &ptm;

    //open the file with proper errors
    file = new std::fstream();
    
    file->open(filename, std::fstream::in);

    if (!file || !file->is_open()) {
        throw std::runtime_error("File could not be opened");
    }
    
    //initialize member variables
    line = 1;
}

Process::~Process() {
    if (file && file->is_open()) {
        file->close();
    }

    delete file;
    delete vm_pmcb;
    
    file = nullptr;
    vm_pmcb = nullptr;
}

void Process::Exec(){
    //May be safe to assume we don't need to switch to user mode immediately
    //since the first line will do alloc, which does just that.

    //extracts the lines from the file one at a time and performs commands
    std::string readLine = "";
        
    while (getline(*file, readLine)){
        //prints out the line number and the command being read
        std::cout << std::dec << line << ":" << readLine << "\n";

        std::istringstream s(readLine); 
        int memAddress; 
        std::string word;
        
        //store the memory address for use and then move to next word 
        s >> std::hex >> memAddress;
        
        //store the command for parsing
        s >> word;
        
        //determine which command to execute
        //alloc command
        if (word == "alloc"){
            int pages;
            s >> std::hex >> pages;

            alloc(memAddress, pages);
        } 

        //cmp command
        else if (word == "cmp"){
            int memAddress2;
            int count; 
            s >> std::hex >> memAddress2;
            s >> std::hex >> count;

            cmp(memAddress, memAddress2, count);
        }

        // set command
        else if (word == "set"){
            std::vector<std::uint8_t> v;
            int value;
            
            //collect all the values that we need to set into a vector
            while(s){
                if (s >> std::hex >> value) {
                    v.push_back(value);
                }
            }
                                   
            set(memAddress, v);
        }
        
        //fill command
        else if (word == "fill") {
            int value;
            int count;
            s >> std::hex >> value;
            s >> std::hex >> count;
            fill(memAddress, value, count);
        }
        
        //dup command
        else if (word == "dup") {
            int dest_address;
            int count;
            s >> std::hex >> dest_address;
            s >> std::hex >> count;
            dup(memAddress, dest_address, count);
        }
        
        //print command
        else if (word == "print") {
            int count;
            s >> std::hex >> count;
            
            print(memAddress, count);
        }

        //perm command
        else if (word == "perm") {
            int count;
            bool status;
            s >> std::hex >> count;
            s >> status;

            perm(memAddress, count, status);
        }

        //increments the counter for line. Leave at the end
        line++;
    }
}

void Process::alloc(int address, int pages) {
    //if we don't have a pmcb yet make one
    if (!vm_pmcb) {
        vm_pmcb = new mem::PMCB(ptm->buildUserPageTable(address));
    }
    
    //switch to kernel mode, allocate pages, and go back to user mode
    mem->set_kernel_PMCB();
    ptm->allocate(pages, vm_pmcb->page_table_base, address);
    mem->set_user_PMCB(*vm_pmcb);

}

void Process::cmp(int address1, int address2, int count) {
    //check each memory address
    for (int offset = 0; offset < count; offset++) {
        //convenience variables for cleaner code
        mem::Addr addressA = address1 + offset;
        mem::Addr addressB = address2 + offset;
        std::uint8_t valA;
        std::uint8_t valB;
        //get from memory version
        //if any of these fail - we have a fault stop executing the command
        if(!mem->movb(&valA, addressA)) break;
        if(!mem->movb(&valB, addressB)) break;
        
        //if mismatch found, print information
        if (valA != valB) {
            std::cout << "cmp error, addr1 = " << std::setfill('0')
                    << std::setw(7) << std::hex << addressA << ", value = "
                    << std::setfill('0') << std::setw(2) << std::hex
                    << (std::uint32_t) valA << ", addr2 = " << std::setfill('0')
                    << std::setw(7) << std::hex << addressB << ", value = "
                    << std::setfill('0')<< std::setw(2) << std::hex
                    << (std::uint32_t) valB << "\n";
        }
    }
}

void Process::set(int address, std::vector<std::uint8_t> &v){
    //keep track of how far away we are as an offset
    //then assign the values
    for(int offset = 0; offset < v.size(); offset++){
        mem::Addr addr = address + offset;
        std::uint8_t val = v.at(offset);
        //add to the memory
        //if this fails stop executing the command
        if(!mem->movb(addr, &val)) break;
    }
}

void Process::fill(int address, std::uint8_t value, int count) {
    //fill the vector of memory from the front with a value count times
    for (int i = 0; i < count; i++) {
        mem::Addr addr = address + i;
        //if this fails stop executing this command
        if(!mem->movb(addr, &value)) break;
    }
}

void Process::dup(int src_address, int dest_address, int count) {
    //find the value at the address and copy it to the new one, use i as offset
    for (int i = 0; i < count; i++) {
        std::uint8_t val;
        mem::Addr curr_src = src_address +  i;
        mem::Addr curr_dest = dest_address +  i;
        //if any retrievals or stores fail stop executing this command
        if(!mem->movb(&val, curr_src)) break;
        if(!mem->movb(curr_dest, &val)) break;
    }
}

void Process::perm(int address, int pages, bool status) {
    //go into kernel mode, set the writable bits and go back to user mode
    mem->set_kernel_PMCB();
    ptm->setWritable(vm_pmcb, address, pages, status);
    mem->set_user_PMCB(*vm_pmcb);
}

void Process::print(int address, int count){
    
    for(int i = 0; i < count; i ++){
        //print the header and break into new lines when needed
        if(i%16 == 0){
            if(i > 0){
                std::cout<< '\n';
            }
            std::cout << std::setfill('0') << std::setw(7) << std::hex 
                 << address + i << ":";
        }
        
        mem::Addr addr = address + i;
        std::uint8_t val;
        //if referencing that part of memory causes a fault then break
        //this command loop
        if(!mem->movb(&val, addr, 1)) return;
        
        //print the address that we just received
        std::cout << " " << std::setfill('0') << std::setw(2) << std::hex 
            << (int) val;
        
    }
    
    std::cout << '\n';
    
}
