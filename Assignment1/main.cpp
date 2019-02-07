/* 
 * File:   main.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on January 24, 2019, 9:53 PM
 */


#include "scheduler.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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
    process p = {name, arrivalTime, totalTime, 
        blockInterval, -1, 0, 0};
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
    
    
    /*
    std::cout << "start of the testing code" << '\n';
    std::vector<process> testCase;
    process tester1 = {"test", 1, 2, 6, 0, false, 0, 0};
    process tester2 = {"test2", 4, 5, 10, 0, false, 0, 4};
    process tester3 = {"test3", 7, 8, 9, 0, false, 0, 0};
    testCase.push_back(tester1);
    testCase.push_back(tester2);
    testCase.push_back(tester3);
    std::priority_queue<process, std::vector<process>, processOperators> c3(testCase.begin(), testCase.end());
    std::cout<< c3.top().name << '\n';
    c3.pop();
    std::cout<< c3.top().name << '\n';
    c3.pop();
    std::cout<< c3.top().name << '\n';
    std::cout << "end of the testing code" << '\n';
    
    return 0;
    */
    
    //open the file and throw errors as needed
    std::ifstream myFile(filename);
    if (!myFile.is_open() || !myFile.good()) {
        throw std::runtime_error{ "Error: failed to open file: " + filename };
    }
    
        
    //fill a vector with process structs containing all information 
    std::vector<process> processVector;
    std::string processInfo = "";
    while(getline(myFile, processInfo)){
        process p = parseProcess(processInfo);
        processVector.push_back(p);
    }

    scheduler s(processVector, blockDuration, timeSlice);

    //TODO: add round robin
    std::cout << "RR " << blockDuration << " " << timeSlice << "\n";
    s.rr();

    //print header for the algorithm
    std::cout << "SPN " << blockDuration << "\n";
    s.spn();
    
    //close the file and end the program
    if (myFile && myFile.is_open()) myFile.close();
    
    
    return 0;
}


