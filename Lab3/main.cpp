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
    FrameAllocator f2(10);
    return 0;
    
    //check for the proper number of args
    if (argc != 2) {
        std::cerr << "Usage: Lab3 file" << std::endl;
        
        return 1;
    }
    
    //store the filename for use throughout the read.
    std::string filename = argv[1];
    
    //create and open the file
    std::ifstream myFile(filename);
    if (!myFile.is_open() || !myFile.good()) {
        throw std::runtime_error{ "Error: failed to open file: " + filename };
    } else {
     
        //read the first value in the file and store it
        std::string readLine = "";
        getline(myFile, readLine);
        std::cout << "|" << readLine << "\n";
        std::istringstream s(readLine); 
        int frameNumber; 
        s >> frameNumber;
        
        //build the frame allocator 
        FrameAllocator f(frameNumber);
        
        readLine = "";
        
        
        //cycle through and print each line and do the required commands
        while (getline(myFile, readLine)){
            //prints out the line number and the command being read
            std::cout << "|" << readLine << "\n";
            
            //set up a way to read in the values for each line not
            //not related to the one before
            std::istringstream s1(readLine); 
            int processNumber; 
            int pageFrameCount;
            std::string command;
            
            //get the command we need to parse
            s1 >> command;
            
            
            if(command == "A"){
                //store the needed values
                s1 >> std::hex >> processNumber;
                s1 >> std::hex >> pageFrameCount;
            }
            
            else if(command == "R"){
                //store the needed values
                s1 >> std::hex >> processNumber;
                s1 >> std::hex >> pageFrameCount;
            }
            
            else if(command == "P"){
                
            }
            
            else {
                throw std::runtime_error{ "Error: unknown command found: " + command };
            }
            
        }
        
         
        //close the file now that we have read through the whole thing
        myFile.close();
    } 

    return 0;
}

