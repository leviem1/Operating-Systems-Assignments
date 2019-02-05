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

struct process{
    std::string name;
    int arrivalTime;    // time process arrives
    int totalTime;      // time process needs to end
    int blockInterval;  // how often the process will be blocked
    int blockTimeTotal; // if >= 0 then amount time its been blocked. = -1 not blocked
    int turnaround;     //total amount of time between arrival and completion
    int runningTime;    // if > 0 then amount time running. = 0 its hasn't started
};

class processOperators{
public:
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
    
   
    scheduler(std::vector<process> &p, int bDuration, int slice);
    
    ~scheduler();
    
    scheduler(scheduler&& other) = delete;
    scheduler& operator=(const scheduler& other) = delete;
    scheduler(const scheduler& other) = delete;
    scheduler& operator=(scheduler&& other) = delete;
    
    void spn();
    void rr();
    
    
private:
    
    int blockedDuration;
    int timeSlice;
    std::vector<process> processes;
    
    

    
    
   
};

#endif /* SCHEDULER_H */

