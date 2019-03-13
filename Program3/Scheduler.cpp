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
        
        for (std::list<Process>::iterator it = processes->begin(); it != processes->end(); it++){
            bool complete = it->Exec(timeSlice);
            
            if(complete){
                processes->remove(*it);
            }
        }
        
        /*
        //execute the process for the time slice
        bool complete = processes->begin()->Exec(timeSlice);
        
        //if it terminates remove it
        if (complete){
            processes->pop_front();
        } 
        //otherwise add it to the back and delete from the front
        else {
            
            processes->emplace_back(processes->front());
            processes->pop_front();
        }
         */
    }
    
}

