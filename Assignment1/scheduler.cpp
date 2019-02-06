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
    
    std::vector<process> workingCopy = processes;
    
    std::priority_queue<process, std::vector<process>, processOperators> 
        queue;
    
    //if there's stuff to do
    while(!workingCopy.empty()){
        
        /*******BEGIN LIST PREPARATION*******/
        for(int i = 0; i < workingCopy.size(); i++){
            process &temp = workingCopy.at(i);
                        
            //check to see if there is something new that has arrived
            //push to the ready queue
            if(temp.arrivalTime == time){
                queue.push(temp);
                continue;
            }
            
            //check if something has unblocked and push to ready queue
            //reset the blocked time to 0
            if(temp.blockTimeTotal == blockedDuration){
                temp.blockTimeTotal = -1;
                queue.push(temp);
                continue;
            }
        }
        
        /********END LIST PREPARATION********/
        /********BEGIN LIST PROCESSING*******/
        
        process curr = queue.top();
        queue.pop();
        for(int i = 0; i < workingCopy.size(); i ++){
            process &temp = workingCopy.at(i);
            
            //now that we have checked the list based on past state
            //increment the counter for rest of the blocked things
            if (temp.blockTimeTotal >= 0) {
                temp.blockTimeTotal++;
            }
            
            //find match to running process and "run" it
            if (temp.name == curr.name) {
                //Assume new process
                if (temp.runningTime == 0 ) {
                    std::cout << time << "\t" << temp.name << "\t";
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
                    queue.push(temp);
                }
            }

            //check if the thing arrived already before updating turnaround
            if (time >= temp.arrivalTime) {
                temp.turnaround++;
            }
            
            
            /********END LIST PROCESSING*********/
        }
        
        //increment the passage of time
        time++;
    }
    
    double averageT = 0.0;
    averageT = spnTurnSum / (double) processes.size();
    
    std::cout << time << "\t<done>\t" << averageT << "\n"; 

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

