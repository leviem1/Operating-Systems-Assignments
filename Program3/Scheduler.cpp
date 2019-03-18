/* 
 * File:   Scheduler.cpp
 * Author: Elisabeth Bristol and Levi Muniz 
 * 
 * Created on March 13, 2019, 2:04 PM
 */

#include "Scheduler.h"


Scheduler::Scheduler(int timeSlice, std::list<Process> &processes) {
    this->timeSlice = timeSlice;
    this->processes = &processes;
}

void Scheduler::roundRobin(){
    
    //go through processes until nothing left
    while (!processes->empty()){
        auto it = processes->begin();

        //while we haven't reached the end of the list
        while (it != processes->end()) {
            //execute the process 
            bool complete = it->Exec(timeSlice);
            
            //if it's done remove it otherwise move the iterator up
            if(complete){
                it = processes->erase(it);
            }
            else {
                it ++;
            }
        }
    }  
}

