/* 
 * File:   spn.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 * 
 * Created on February 4, 2019, 4:21 AM
 */

#include "spn.h"

spn::spn(std::vector<std::string> &pInfo, int bDuration) {
    processInfo = pInfo;
    blockDuration = bDuration;
    simTime = 0;
}

void spn::run(){
    
}

std::string spn::parseProcessName(std::string process){
    //extract the name from the process string
    std::string name;
    std::istringstream s(process); 
    s >> name;
    return name;  
}

std::vector<std::string> spn::parseProcessNum(std::string process){
    //the strings as they occur in the format of the file
    std::string name;
    int arrivalTime;
    int totalTime;
    int blockInterval;
    
    //create the string stream to break up the original process string
    std::istringstream s(process); 
    s >> name;
    s >> arrivalTime;
    s >> totalTime;
    s >> blockInterval;
    
    //create a fill the vector of strings to return
    std::vector<std::string> p;
    p.at(0) = arrivalTime;
    p.at(1) = totalTime;
    p.at(2) = blockInterval;
    
    return p;
}

spn::~spn() {
}

