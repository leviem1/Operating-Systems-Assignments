/* 
 * File:   Process.cpp
 * Author: Levi Muniz
 * 
 * Created on January 18, 2019, 4:44 PM
 */

#include "Process.h"

Process::Process(const std::string &filename) {
    file = new std::fstream();
    
    file->open(filename, std::fstream::in);

    if (!file || !file->is_open()) {
        throw std::runtime_error("File could not be opened");
    }
    
    line = 1;
 }



Process::~Process() {
    if (file && file->is_open()) {
        file->close();
    }
    
    delete file;
    file = nullptr;
}

void Process::Exec(){
    //extracts the lines from the file one at a time and performs commands
    std::string readLine = "";
        
    while (getline (*file, readLine)){
        //prints out the line number of the command being read in decimal
        std::cout << std::dec << line << ":" << readLine << "\n";

        std::istringstream s(readLine); 
        int memAddress; 
        std::string word;
        
        //store the memory address for use and then move to next word 
        s >> std::hex >> memAddress;
        
        //store the command for parsing
        s >> word;
        
        //determine which command to execute
        //memsize command first line non commented in file
        if (word == "memsize"){
            std::cout << memAddress << "\n";
            memsize(memAddress);
        } 

        else if (word == "cmp"){
            int memAddress2;
            int count; 
            s >> std::hex >> memAddress2;
            s >> std::hex >> count;

            cmp(memAddress, memAddress2, count);
        }

        //this branch adds dealing with the set command
        else if (word == "set"){
        }

        //This is our catch all until everything is done
        else {
            std::cout << "This line not addressed yet.\n";
        }
        //TODO: rest of commands

        //increments the counter for line. Leave at the end
        line++;
    }
}

void Process::memsize(int address){
    //check that it follows program constraints
    if (address > 4000000){
        throw std::runtime_error{ "Error: memsize command address size is too large"};
    }
    
    //initialize and fill constructor with zeros
    mem = new std::vector<uint8_t>(address, 0);
}

void Process::cmp(int address1, int address2, int count) {
    //check each memory address
    for (int offset = 0; offset < count; offset++) {
        
        //convenience variables for cleaner code
        int addressA = address1 + offset;
        int addressB = address2 + offset;
        uint8_t valA = mem->at(addressA);
        uint8_t valB = mem->at(addressB);
        
        //if mismatch found, print information
        if (valA != valB) {
            std::cerr << "cmp error, addr1 = " << std::hex << addressA
                    << ", value = " << std::hex << valA
                    << ", addr2 = " << std::hex << addressB
                    << ", value = " << std::hex << valB
                    << "\n";
        }
    }
    
    
}