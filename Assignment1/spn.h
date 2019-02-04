/* 
 * File:   spn.h
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on February 4, 2019, 4:21 AM
 */

#ifndef SPN_H
#define SPN_H

#include <stdlib.h>

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <queue>

class spn {
public:
    spn(std::vector<std::string> &pInfo, int bDuration);
    ~spn();
    
    //explicit rule of five deletes
    spn(const spn& other) = delete;
    spn(spn&& other) = delete;
    spn& operator=(const spn& other) = delete;
    spn& operator=(spn&& other) = delete;
    
    void run();
    
private:
    std::vector<std::string> processInfo;
    int blockDuration;
    int simTime;
    std::priority_queue<int> readyList;
    std::priority_queue<int> blockedList;
    
    
    
    std::vector<std::string> parseProcessNum(std::string process);
    std::string parseProcessName(std::string process);
    

};

#endif /* SPN_H */

