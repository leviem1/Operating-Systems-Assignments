/* 
 * File:   main.cpp
 * Authors: Elisabeth Bristol and Levi Muniz
 *
 * Created on January 14, 2019, 1:27 PM
 */

#include <iostream>
#include "Process.h"

using namespace std;

int main(int argc, char** argv) {
    
    //check for the proper number of arguments
    if (argc != 2) {
        std::cerr << "Usage: Lab4 file" << std::endl;
        
        return 1;
    }
    
    //create the object and run exec on the file with given commands
    Process p(argv[1]);
    p.Exec();

    return 0;
}

