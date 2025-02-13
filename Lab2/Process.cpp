/* 
 * File:   Process.cpp
 * Authors: Elisabeth Bristol and Levi Muniz
 * 
 * Created on January 18, 2019, 4:44 PM
 */

#include "Process.h"


Process::Process(const std::string &filename) {
    //open the file with proper errors
    file = new std::fstream();
    
    file->open(filename, std::fstream::in);

    if (!file || !file->is_open()) {
        throw std::runtime_error("File could not be opened");
    }
    
    //initialize member variables
    line = 1;
    mem = new std::vector<std::uint8_t>;  
 }

Process::~Process() {
    if (file && file->is_open()) {
        file->close();
    }
    
    if (mem) delete mem;
    if (file) delete file;
    
    file = nullptr;
    mem = nullptr;
}

void Process::Exec(){
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
        //memsize command
        if (word == "memsize"){
            memsize(memAddress);
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
                s >> std::hex >> value;
                v.push_back(value);
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

        //increments the counter for line. Leave at the end
        line++;
    }
}

void Process::memsize(int address){
    //check that it follows program constraints
    if (address > 4000000){
        throw std::runtime_error{ "Error: memsize command address "
                "size is too large"};
    }
    
    //initialize and fill vector with zeros
    mem->resize(address);
    for(int i = 0; i < mem->size() - 1; i++){
        mem->at(i) = 0;
    }
}

void Process::cmp(int address1, int address2, int count) {
    //check each memory address
    for (int offset = 0; offset < count; offset++) {
        //convenience variables for cleaner code
        int addressA = address1 + offset;
        int addressB = address2 + offset;
        int valA = mem->at(addressA);
        int valB = mem->at(addressB);
        
        //if mismatch found, print information
        if (valA != valB) {
            std::cerr << "cmp error, addr1 = " << std::setfill('0')
                    << std::setw(7) << std::hex << addressA << ", value = "
                    << std::setfill('0') << std::setw(2) << std::hex << valA
                    << ", addr2 = " << std::setfill('0') << std::setw(7)
                    << std::hex << addressB << ", value = " << std::setfill('0')
                    << std::setw(2) << std::hex << valB << "\n";
        }
    }
}

void Process::set(int address, std::vector<std::uint8_t> &v){
    //keep track of how far away we are as an offset
    //then assign the values
    for(int offset = 0; offset < v.size() - 1; offset++){
        mem->at(address + offset) = v.at(offset);
    }
}

void Process::fill(int address, int value, int count) {
    //fill the vector of memory from the front with a value count times
    std::fill_n(mem->begin() + address, count, value);
}

void Process::dup(int src_address, int dest_address, int count) {
    //find the value at the address and copy it to the new one, use i as offset
    for (int i = 0; i < count; i++) {
        mem->at(dest_address +  i) = mem->at(src_address +  i);
    }
}

void Process::print(int address, int count){
    
    //markers to keep track of where we are
    int placeHolderTotal = 0;
    int placeHolderRow = 0;
    
    //outer loop keeps track of building the headers and breaks fro new lines
    for(int i = 0; i < count; i +=16){
        placeHolderRow = 0;
         std::cout << std::setfill('0') << std::setw(7) << std::hex 
            << address + i << ":";
         
         //so long as we have stuff to print that fits in the row
         //print it accounting for total changes
         for(int j = 0; (placeHolderRow < 16 && j < count ); j ++){
              std::cout << " " << std::setfill('0') << std::setw(2) << std::hex 
                    << (int) mem->at(address + placeHolderTotal + j);
              placeHolderRow ++;  
        }
         
         //update counter and new line break
         placeHolderTotal += 16;
          std::cout << "\n"; 
    }   
}