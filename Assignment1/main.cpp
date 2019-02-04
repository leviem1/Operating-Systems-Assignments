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

process parseProcess(std::string processInfo){
    //the strings as they occur in the format of the file
    std::string name;
    int arrivalTime;
    int totalTime;
    int blockInterval;
    
    //create the string stream to break up the original process string
    std::istringstream s(processInfo); 
    s >> name;
    s >> arrivalTime;
    s >> totalTime;
    s >> blockInterval;
    
    //create a process to return 
    process p(name, arrivalTime, totalTime, blockInterval);
    return p;
}

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
    
    //check to make sure they got converted correctly
    if(timeSlice == 0L){
        std::cerr << "Conversion error on timeSlice argument\n";
    }
    if(blockDuration == 0L){
        std::cerr << "Conversion error on timeSlice argument\n";
    }
    
    //open the file and throw errors as needed
    std::ifstream myFile(filename);
    if (!myFile.is_open() || !myFile.good()) {
        throw std::runtime_error{ "Error: failed to open file: " + filename };
    }
    
    else{
        
         
        //fill a vector with process objects containing all information 
        std::vector<process> processVector;
        std::string processInfo = "";
        while(getline(myFile, processInfo)){
            process p = parseProcess(processInfo);
            processVector.push_back(p);
        }
        
        
        //TODO: add round robin
        std::cout << "RR" << blockDuration << " " << timeSlice << "\n";
        
        //print header for the algorithm
        std::cout << "SPN " << blockDuration << " " << timeSlice << "\n";
        
        //execute shortest process next on the input
        spn s(processVector, blockDuration);
        s.run();
        
    }
    
    //close the file and end the program
    myFile.close();
    
    
    return 0;
}


