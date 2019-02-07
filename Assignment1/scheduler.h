/* 
 * File:   scheduler.h
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on February 5, 2019, 12:19 AM
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>

//represents each process specified in the file with all known data 
struct process{
    std::string name;
    int arrivalTime;    // time process arrives
    int totalTime;      // time process needs to end
    int blockInterval;  // how long the process runs before blocking
    int blockTimeTotal; // if >= 0 then amount time its been blocked. = -1 not blocked
    int turnaround;     //total amount of time between arrival and completion
    int runningTime;    // if > 0 then amount time running. = 0 its hasn't started
    int sliceTime; //total amount of time on CPU regardless of slices
};


class processOperators{
public:
    /**
     * One process struct is defined as less than another if
     * the running time of that process is greater than 0
     * indicating that process is being "run" OR
     * ones block interval - being compared for spn algorithm - is smaller
     * @param p1 - process struct
     * @param p2 - process struct
     * @return bool - does it meet the less then criteria for the queue
     */
    bool operator()(process p1, process p2){
        if(p1.runningTime > 0){
            return false;
        } else if(p2.runningTime > 0){
            return true;
        }
        else {
            return p1.blockInterval > p2.blockInterval;
        }
    }
};


class scheduler {
public:

    /**
     * Constructor for a scheduler that can run two algorithms 
     * 1. round robin 2. shortest process next
     * @param p - vector of process structs that contains all 
     * information based on file
     * @param bDuration - constant length of block time
     * @param slice - length of the time slice for round robin
     */
    scheduler(std::vector<process> &p, int bDuration, int slice);
    
    //obligatory empty destructor
    ~scheduler();
    
    //explicit deletes
    scheduler(scheduler&& other) = delete;
    scheduler& operator=(const scheduler& other) = delete;
    scheduler(const scheduler& other) = delete;
    scheduler& operator=(scheduler&& other) = delete;
    
    /**
     * function spn - runs the shortest process next algorithm on the struct 
     * data list passed to the scheduling object. Uses the given constant of
     * block duration
     * @return - void
     */
    void spn();
    
    /**
     * function rr- runs the round robin algorithm on the struct 
     * data list passed to the scheduling object. Uses the given constants of
     * block duration and time slice
     * @return - void
     */
    void rr();
    
    
private:
    
    //constants provided by the file
    int blockedDuration;
    int timeSlice;
    //vector containing all process information as given by the file
    std::vector<process> processes;
   
};

#endif /* SCHEDULER_H */

