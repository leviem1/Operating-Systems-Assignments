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
            std::cout << std::dec << line << ":";
            
            //allows us to look at the words in the line one at a time 
            std::istringstream s(readLine); 
            std::string word; 
            s >> word;
                //search out the lines with comments
            if (word == "*"){
                 std::cout << "*";
                  do { 
                      s >> word;
                      std::cout << word << " ";
                  } while (s);
                  std::cout << '\n';
                        
            } 
            //print blanks lines where applicable
            else if(word == ""){
                std::cout << '\n';
            }
            //now we have to deal with commands
            else if(word != ""){
                //store the memory address for use and then move to next word 
                //determine which command to execute
                std::string memAddress = word;
                s >> word;
                //memsize command first line non commented in file
                if (word == "memsize"){
                    
                } 
                //TODO: Levi -> you can start from here
                else if (word == "cmp"){
                    
                }
            
                
                
            }
            //TODO: rest of commands
            else {
                std::cout << "This line not addresses yet. " << '\n';
            } 
            
            //increments the counter for line. Leave at the end
            
            line ++;
            
        }
   
}