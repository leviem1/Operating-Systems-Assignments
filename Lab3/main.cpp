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
#include <vector>

#include "FrameAllocator.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    //check for the proper number of args
    if (argc != 2) {
        cerr << "Usage: Lab3 file" << endl;
        
        return 1;
    }
    
    //store the filename for use throughout the read.
    string filename = argv[1];
    
    //create and open the file
    ifstream myFile(filename);
    if (!myFile.is_open() || !myFile.good()) {
        throw runtime_error{ "Error: failed to open file: " + filename };
    } else {
     
        //read the first value in the file and store it
        string readLine = "";
        getline(myFile, readLine);
        cout << "|" << readLine << "\n";
        istringstream s(readLine); 
        int frameNumber; 
        s >> frameNumber;
        
        //build the frame allocator 
        FrameAllocator f(frameNumber);
        
        //vector of vectors that store all the processes
        std::vector<std::vector<uint32_t>> processes(4);
        
        readLine = "";
        
        bool success = false;
        
        
        //cycle through and print each line and do the required commands
        while (getline(myFile, readLine)){
            //prints out the line number and the command being read
            cout << "|" << readLine << "\n";
            
            //set up a way to read in the values for each line not
            //not related to the one before
            istringstream s1(readLine); 
            uint32_t processNumber; 
            uint32_t pageFrameCount;
            string command;
            
            //get the command we need to parse
            s1 >> command;
            
            
            if(command == "A"){
                //store the needed values
                s1 >> hex >> processNumber;
                s1 >> hex >> pageFrameCount;
                success = f.Allocate(pageFrameCount, processes.at(processNumber));
                if(success){
                    std::cout << " T " << std::hex << f.get_available() << "\n";
                } 
                
                else {
                    std::cout << " F " << std::hex << f.get_available() << "\n"; 
                }
            }
            
            else if(command == "R"){
                //store the needed values
                s1 >> hex >> processNumber;
                s1 >> hex >> pageFrameCount;
                success = f.Release(pageFrameCount, processes.at(processNumber));
                
                if(success){
                    std::cout << " T " << std::hex << f.get_available() << "\n";
                } 
                
                else {
                    std::cout << " F " << std::hex << f.get_available() << "\n"; 
                }
            }
            
            else if(command == "P"){
                
                std::cout << f.get_available_list_string() << "\n";
                
            }
            
            else {
                throw runtime_error{ "Error: unknown command found: " + command };
            }
            
        }
        
         
        //close the file now that we have read through the whole thing
        myFile.close();
    } 

    return 0;
}

