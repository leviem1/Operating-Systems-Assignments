/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: muniz
 *
 * Created on January 29, 2019, 10:10 AM
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "FrameAllocator.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
     //check for the proper number of args
    if (argc != 2) {
        std::cerr << "Usage: Lab3 file" << std::endl;
        
        return 1;
    }
    
    //store the filename for use throughout the read.
    std::string filename = argv[1];
    
    //create and open the file
    std::ifstream myFile(filename);
    if (!myFile.is_open()) {
        throw std::runtime_error{ "Error: failed to open file: " + filename };
    } else {
     
        //read the first value in the file and store it
        std::string readLine = "";
        getline(myFile, readLine);
        std::istringstream s(readLine); 
        int frameNumber; 
        s >> frameNumber;
        
        //build the frame allocator 
        FrameAllocator f(frameNumber);
        
         
        //close the file now that we have read through the whole thing
        myFile.close();
    } 

    return 0;
}

