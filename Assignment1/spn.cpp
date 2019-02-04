/* 
 * File:   spn.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 * 
 * Created on February 4, 2019, 4:21 AM
 */

#include "spn.h"
    

spn::spn(std::vector<process> &p, int bDuration) {
    processes = p;
    blockDuration = bDuration;
    simTime = 0;
    running = true;
    
   
}

void spn::run(){
    
    while(running){
        
        //run through our list of processes and see if a new one has started
        for(int i = 0; i < processes.size(); i++){
            if(processes.at(i).getArrivalTime() == simTime){
                //readyList.push(processes.at(i));
            } 
            
            //temporary for testing
            running = false;
            
            //this acts like our clock over the while loop
            simTime = simTime + 1;
        }
    } 
}


spn::~spn() {
}

