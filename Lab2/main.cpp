/* 
 * File:   main.cpp
 * Authors: Elisabeth Bristol and Levi Muniz
 *
 * Created on January 14, 2019, 1:27 PM
 */

#include <iostream>
#include "Process.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: Lab2 file" << std::endl;
        
        return 1;
    }
    
    Process p(argv[1]);
    p.Exec();

    return 0;
}

