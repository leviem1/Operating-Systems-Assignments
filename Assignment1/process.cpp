/* 
 * File:   process.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 * 
 * Created on February 4, 2019, 5:56 AM
 */

#include "process.h"

process::process(std::string n, int at, int tt, int bt) {
    name = n;
    arrivalTime = at;
    totalTime = tt;
    blockInterval = bt;
}



bool process::operator<(const process& rhs){
    if(rhs.blockInterval < this->blockInterval){
        return true;
    } else {
        return false;
    }
}

int process::getArrivalTime(){
    return arrivalTime;
}

int process::getBlockInterval(){
    return blockInterval;
}

int process::getTotalTime(){
    return totalTime;
}

std::string process::getName(){
    return name;
}

process::~process() {
}

process::process(process&& other){
    
}

process& process::operator=(const process& other){
    
}

process::process(const process& other){
    
}

process& process::operator=(process&& other){
    
}
