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
    double spnTurnSum = 0;
    
    std::vector<process> workingCopy = processes;
    
    std::priority_queue<process, std::vector<process>, processOperators> 
        queue;
    
    //if there's stuff to do
    while(workingCopy.empty() == false){
        
        //check to see if there is something new that has arrived
        //push to the ready queue
        for(int i = 0; i < workingCopy.size(); i ++){
            if(workingCopy.at(i).arrivalTime == time){
                process temp = workingCopy.at(i);
                queue.push(temp);
            }
            
        }
        
        //check if something has unblocked and push to ready queue
        //reset the blocked time to 0
        //set it to ready
        for(int i = 0; i < workingCopy.size(); i ++){
            if(workingCopy.at(i).blockTimeTotal == blockedDuration){
                workingCopy.at(i).blockTimeTotal = -1;
                process temp = workingCopy.at(i);
                queue.push(temp);
            }
            //now that we have checked the list based on past state
            //increment the counter for rest of the blocked things
            if(workingCopy.at(i).blockTimeTotal >= 0){
                workingCopy.at(i).blockTimeTotal ++;
            }
        }
        
        //now we pop the shortest thing off the queue
        process temp = queue.top();
        queue.pop();
        //go through and try to find a match on the name
        for(int i = 0; i < workingCopy.size(); i ++){
            std::string tname = temp.name;
            process &p = workingCopy.at(i);
            //when we find the match increment running time
            if(p.name == tname){
                //starts running a process that wasn't before
                //or increment the one that was running before
                //see comparison operator that favors a running process
                p.runningTime ++;
                //now that we have accomplished something take it away from what needs doing on CPU
                p.totalTime --;
                //if the thing won't go to blocked, put it back
                if (p.runningTime < p.blockInterval){
                    queue.push(p);
                } 
                //now the thing is blocked...don't put it back on the queue
                //update the values to reflect it is blocked
                else {
                    p.runningTime = 0;
                    p.blockTimeTotal = 0;
                }
            }
        }
        
        //get stuff out of the master list that has total time completed
        for(int i = 0; i < workingCopy.size(); i ){
            //check if the thing arrived already before update turnaround
            if(time >= workingCopy.at(i).arrivalTime){
                workingCopy.at(i).turnaround ++;
            }
            //take the things finished out of the list
            if(workingCopy.at(i).totalTime == 0){
                //add the turnaround about to be removed to sum for average
                spnTurnSum += workingCopy.at(i).turnaround;
                workingCopy.erase(workingCopy.begin() + i);
            }
        }
        
        //increment the passage of time
        time ++;
    }

}

void scheduler::rr(){
    
    int time = 0;
    std::string nameRunning = "";
    int cycleCounter;
    
    std::vector<process> wc = processes;
    std::vector<process> ready;
    std::vector<process> blocked;
    
    while(wc.empty() == false){
    
        //if the process arrives add it to the ready list.
        for(int i = 0;  i < wc.size(); i++){
            if(wc.at(i).arrivalTime == time){
                ready.push_back(wc.at(i));
            }
        }
        
        //get the thing that should be running for printing purposes
        process &running = ready.at(0);
        if (ready.size() == 0){
            nameRunning = "<idle>";
        }
        //if the thing hasn't finished its time slice yet then have it run again
        else if(cycleCounter < timeSlice){
            nameRunning = running.name;
            running.totalTime --;
            cycleCounter ++;
            //case where it runs the whole way through and goes back to end of list
            if(cycleCounter == timeSlice){
                cycleCounter = 0;
                //put it on the end of the list and remove the bad one
                ready.push_back(running);
                ready.erase(ready.begin()); 
            }
            else if(running.totalTime == 0){
                ready.erase(ready.begin());
                
            }
        }
        else {
            
        }
        
        
    }
        
}

