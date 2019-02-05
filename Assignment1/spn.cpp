/* 
 * File:   spn.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 * 
 * Created on February 4, 2019, 4:21 AM
 */

/*
#include "spn.h"
    

spn::spn(std::vector<process> &p, int bDuration) {
    processes = p;
    blockDuration = bDuration;
    simTime = 0;
    running = true;
    CPUBusy = false;
    
   
}


bool spn::isComplete(std::vector<process> p){
    //go through all the processes to see if they are complete
    for(int i = 0; i < p.size(); i ++){
        if (p.at(i).getComplete() == false){
            return false;
        }
    }
    
    return true;
}

void spn::run(){
    
    while(isComplete(processes)){
        
        process *p = NULL;
      
        
        //run through our list of processes and see if a new one has arrived
        for(int i = 0; i < processes.size(); i++){
            //check to see if new process has arrived and if so add to readyList
            if(processes.at(i).getArrivalTime() == simTime){
                readyList.push(processes.at(i));
            } 
        }
        
        //check if anything in the blocked queue unblocks
        //if so add to the ready queue
        for(int i = 0; i < blockedList.size(); i ++){
            if(blockedList.at(i).getTimeBlocked() == 0){
                process temp = blockedList.at(i);
                readyList.push(temp);
                blockedList.erase(blockedList.begin() + i);
            }
        }
        
        //if the CPU is unoccupied give it something to do
        if(CPUBusy = false){
            CPUBusy = true;
            //this is the f'd up part
            //p = readyList.top();
            //readyList.pop();
        }
        
        
            
            
        //this acts like our clock over the while loop
        simTime = simTime + 1;
            
        //temporary for testing
        break;
        
    } 
}


spn::~spn() {
}

*/