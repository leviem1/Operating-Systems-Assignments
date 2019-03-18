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


Scheduler::~Scheduler() {
}

void Scheduler::roundRobin(){
    
    
    while (!processes->empty()){
        auto it = processes->begin();

        while (it != processes->end()) {
            bool complete = it->Exec(timeSlice);
            
            if(complete){
                it = processes->erase(it);
            }
            else {
                it ++;
            }
        }
    }  
}

