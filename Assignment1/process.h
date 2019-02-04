/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   process.h
 * Author: elisabethbristol
 *
 * Created on February 4, 2019, 5:56 AM
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class process {
public:
    process(std::string n, int at, int tt, int bt);
    ~process();
    process(process&& other);
    process& operator=(const process& other);
    process(const process& other);
    process& operator=(process&& other);
    
    
    bool operator<(const process& a);
    
    /* f'd up comparison overload
    bool operator<(const process& a, const process& b){
    if(a.getBlockInterval() < b.getBlockInterval()){
        return true;
    } else {
        false;
    }
     
}*/
    
    std::string getName();
    int getArrivalTime();
    int getTotalTime();
    int getBlockInterval();
    
private:
    std::string name;
    int arrivalTime;
    int totalTime;
    int blockInterval;
    
    
};





#endif /* PROCESS_H */

