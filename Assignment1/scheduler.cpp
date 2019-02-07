/* 
 * File:   scheduler.cpp
 * Author: Elisabeth Bristol and Levi Muniz
 * 
 * Created on February 5, 2019, 12:19 AM
 */

#include "scheduler.h"

scheduler::scheduler(std::vector<process> &p, int bDuration, int slice) {
    
    blockedDuration = bDuration;
    timeSlice = slice;
    processes = p;
    
    
}

scheduler::~scheduler() {
}

void scheduler::spn(){
    
    int time = 0;
    int spnTurnSum = 0;
    
    int queueEmptyCounter = 0;
    
    std::vector<process> workingCopy = processes;
    
    std::priority_queue<process, std::vector<process>, processOperators> 
        readyQueue;
    
    //if there's stuff to do
    while(!workingCopy.empty()){
        
        /*******BEGIN LIST PREPARATION*******/
        for(int i = 0; i < workingCopy.size(); i++){
            process &temp = workingCopy.at(i);
                        
            //check to see if there is something new that has arrived
            //push to the ready queue
            if(temp.arrivalTime == time){
                //end of idle time so print the counter value
                if(readyQueue.empty() && time != 0){
                    std::cout << queueEmptyCounter - 1 << "\tI\n";
                }
                readyQueue.push(temp);
                //reset the value
                queueEmptyCounter = 0;
                continue;
            }
            
            //check if something has unblocked and push to ready queue
            //reset the blocked time to 0
            if(temp.blockTimeTotal == blockedDuration){
                temp.blockTimeTotal = -1;
                //the end of idle time is here so print it
                if(readyQueue.empty() && time != 0){
                    std::cout << queueEmptyCounter - 1 << "\tI\n";
                }
                readyQueue.push(temp);
                //reset the idle counter
                queueEmptyCounter = 0;
                continue;
            }
        }
        
        /********END LIST PREPARATION********/
        /********BEGIN LIST PROCESSING*******/
        
        //a way to keep track of if the queue is empty so no
        //segmentations faults
        process curr;
        if(readyQueue.empty() && time != 0){
            curr = {"invalid", -1000, -1000, -1000, -1000, -1000, -1000};
            queueEmptyCounter ++;
        } 
        else {
            curr = readyQueue.top();
            readyQueue.pop();
        }
        
        //going through the list of all processes that need doing
        for(int i = 0; i < workingCopy.size(); i ++){
            process &temp = workingCopy.at(i);
            
            //now that we have checked the list based on past state
            //increment the counter for rest of the blocked things
            if (temp.blockTimeTotal >= 0) {
                temp.blockTimeTotal++;
            }
            
            //check if the thing arrived already before updating turnaround
            if (time >= temp.arrivalTime) {
                temp.turnaround++;
                //std::cout << temp.name << " " << temp.turnaround << '\n';
            }
            
            //if there was actually something in the queue behave normal
            if (curr.name != "invalid"){
                //find match to running process and "run" it
                if (temp.name == curr.name) {
                    //Assume new process
                    if (temp.runningTime == 0 ) {
                        std::cout << " " << time << "\t" << temp.name << "\t";
                    }
                
                    temp.runningTime++;
                    temp.totalTime--;
                
                    //assuming only one thing can ever change
                    //take the things finished out of the list
                    if (temp.totalTime == 0) {
                        std::cout << temp.runningTime << "\tT\n";
                        //add the turnaround about to be removed to sum for average
                        spnTurnSum += temp.turnaround;
                        workingCopy.erase(workingCopy.begin() + i );
                        
//                        std::cout << "after erase \n";
//                        for (int j = 0; j < workingCopy.size(); j++){
//                            //std::cout << workingCopy.at(j).name << '\n';
//                        }
                    
                        //need to move our place in the for loop back one
                        //since the list length is now one shorter
                        i--;
                    
                    }
                
                    //If the process is blocked
                    else if (temp.runningTime == temp.blockInterval) {
                        std::cout << temp.runningTime << "\tB\n";
                        temp.runningTime = 0;
                        temp.blockTimeTotal = 0;
                    }
                
                    else {
                        queueEmptyCounter = 0;
                        readyQueue.push(temp);
                    }
                }
            
            }
            
            //nothing in the queue - deal with idle
            else {
                if (queueEmptyCounter == 1){
                    std::cout << " " << time << "\t" << "<idle>" << "\t";
                    queueEmptyCounter ++;
                }
                
            }
            
            
            /********END LIST PROCESSING*********/
        }
        
        //increment the passage of time
        time++;
    }
    
    double averageT = 0.0;
    averageT = spnTurnSum / (double) processes.size();
    std::cout << " " << time << "\t<done>\t" << averageT << "\n"; 

}

void scheduler::rr(){
    
    int time = 0;
    bool idle = false;
    int idleCount;
    
    std::vector<process> wc = processes;
    std::vector<process> ready;
    
    while(wc.empty() == false){
    
        //prepare the list 
        for(int i = 0;  i < wc.size(); i++){
            process &temp = wc.at(i);
            
            //add new things to the ready list
            if(wc.at(i).arrivalTime == time){
                ready.push_back(wc.at(i));
            }
            
            //add things that have just achieved status unblocked
            if(temp.blockTimeTotal == blockedDuration){
                temp.blockTimeTotal = -1;
                ready.push_back(temp);
                continue;
            } 
            
            //the cpu is idle this cycle 
            if(ready.empty()){
                idle = true;
            }
        }
        
        //process the lists as needed
        for (int i = 0; i < wc.size(); i ++){
            //temp process to keep track of where we are in the list
            process &temp = wc.at(i);
            
            //go through and update the block list 
            //happens whether the CPU is idle or not
            if (temp.blockTimeTotal >= 0) {
                    temp.blockTimeTotal++;
            }
            
            //if we are idle update the counter with how long it has been
            if(idle){
                //this is the first time its gone idle print the thing
                if (idleCount == 0){
                    std::cout << " " << time << "\t" << "<idle>" << "\t";
                }
                idleCount ++; 
            } 
            
            //the CPU is not idle...do stuff
            else {
                process curr = wc.begin();
               
                
                
            }
        }
        
        
    }
        
}

