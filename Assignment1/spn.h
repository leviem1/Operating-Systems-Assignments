/* 
 * File:   spn.h
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on February 4, 2019, 4:21 AM
 */

#ifndef SPN_H
#define SPN_H

#include "process.h"

#include <stdlib.h>

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <queue>

class spn {
public:
    spn(std::vector<process> &p, int bDuration);
    ~spn();
    
    //explicit rule of five deletes
    spn(const spn& other) = delete;
    spn(spn&& other) = delete;
    spn& operator=(const spn& other) = delete;
    spn& operator=(spn&& other) = delete;
    
    void run();
    
private:
    std::vector<process> processes;
    int blockDuration;
    int simTime;
    std::priority_queue<process> readyList;
    std::priority_queue<process> blockedList;
    bool running;
    
   
};

#endif /* SPN_H */

