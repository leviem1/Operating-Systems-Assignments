/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   process.h
 * Author: Elisabeth Bristol
 *
 * Created on February 4, 2019, 5:56 AM
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class process {
public:
    process(std::string n, int at, int tt, int bt, int et, int tb);
    ~process();
    process(process&& other);
    process& operator=(const process& other);
    process(const process& other);
    process& operator=(process&& other);
    
    
    bool operator<(const process& a) const;
    
    
    std::string getName();
    int getArrivalTime();
    int getTotalTime();
    int getBlockInterval();
    bool getComplete();
    int getTimeBlocked();
    
    
private:
    
    std::string name;
    int arrivalTime;
    int totalTime;
    int blockInterval;
    bool complete;
    int endTime;
    int timeBlocked;
    
    
};





#endif /* PROCESS_H */

