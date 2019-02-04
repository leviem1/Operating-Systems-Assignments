/* 
 * File:   main.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on January 24, 2019, 9:53 PM
 */


#include "spn.h"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
    //right number of command arguments?
    if (argc != 4) {
        std::cerr << "Usage: Assignment1 file block_duration time_slice\n";
        return 1;
    }
    
    //collect all info frome the arguments passed
    std::string filename = argv[1];
    //convert the string arguments into integers for use
    int blockDuration = strtol(argv[2], nullptr, 10);
    int timeSlice = strtol(argv[3], nullptr, 10);
    
    //open the file and throw errors as needed
    std::ifstream myFile(filename);
    if (!myFile.is_open() || !myFile.good()) {
        throw std::runtime_error{ "Error: failed to open file: " + filename };
    }
    
    else{
        
        //fill a vector with all processes information
        std::vector<std::string> processes;
        std::string process = "";
        while(getline(myFile, process)){
            processes.push_back(process);
        }
        
        //execute shortest process next on the input
        spn s(processes, blockDuration);
        s.run();
        
        int test = strtol("abced", nullptr, 10);
        
        
    }
    
    
    
    return 0;
}

