/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Scheduler.h
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on March 13, 2019, 2:04 PM
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"

#include <list>

class Scheduler {
public:
    /**
     * Builds a scheduler object
     * @param timeSlice - given from the arguments
     * @param processes - list of all the processes gathered from the 
     * files specified as arguments
     */
    Scheduler(int timeSlice, std::list<Process> &processes);
    
    Scheduler(const Scheduler &other) = delete;  // no copy constructor
    Scheduler(Scheduler &&other) = delete;       // no move constructor
    Scheduler operator=(const Scheduler &other) = delete;  // no copy assign
    Scheduler operator=(Scheduler &&other) = delete;       // no move assign
    ~Scheduler() = default;
    
    /**roundRobin - runs a round robin style algorithm on the processes
     * using the stored timeslice
     * @return void
     */
    void roundRobin();
    
private:
    int timeSlice;
    std::list<Process> *processes;
    
   
};

#endif /* SCHEDULER_H */

