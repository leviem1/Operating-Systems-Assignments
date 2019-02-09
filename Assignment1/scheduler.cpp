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

scheduler::~scheduler() { }

void scheduler::spn(){
    
    int time = 0;
    int spnTurnSum = 0;
    int queueEmptyCounter = 0;
    
    //an editable list of the processes and the queue which is the ready list
    std::vector<process> workingCopy = processes;
    std::priority_queue<process, std::vector<process>, processOperators> 
        readyQueue;
    
    //while loop runs as long as there are things in the working list
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
        
        //a way to keep track of if the queue is empty so no seg faults
        process curr;
        if(readyQueue.empty() && time != 0){
            curr = {"invalid", -1000, -1000, -1000, -1000, -1000, -1000, -1000};
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
    //calculate the average turnaround time and print
    double averageT = spnTurnSum / (double) processes.size();
    std::cout << " " << time << "\t<done>\t" << averageT << "\n"; 
}

void scheduler::rr(){
    int time = 0;
    int rrTurnSum = 0;
    int idleCount = -1;  //if >= 0 how long we've been idle, -1 if not idle

    std::vector<process> wc = processes;
    std::vector<process> ready;
    
    while(!wc.empty()){

        /*******BEGIN LIST PREPARATION*******/
        for(int i = 0;  i < wc.size(); i++){
            process &temp = wc.at(i);

            //add new things to the ready list
            if(temp.arrivalTime == time){
                ready.push_back(temp);
                continue;
            }
            
            //add things that have just achieved status unblocked
            if(temp.blockTimeTotal == blockedDuration){
                temp.blockTimeTotal = -1;
                ready.push_back(temp);
                continue;
            }

            if (temp.sliceTime == -1) {
                ready.push_back(temp);
            }
        }

        /********END LIST PREPARATION********/

        //If no longer idle, print idle time
        if (!ready.empty() && idleCount >= 0) {
            std::cout << idleCount << "\tI\n";
            idleCount = -1;
        }

        //if just became idle, print header
        else if (ready.empty() && idleCount == -1) {
            idleCount = 0;
            std::cout << " " << time << "\t" << "<idle>" << "\t";
        }

        process curr;
        //the CPU is not idle...do stuff
        if (idleCount == -1) {
            //get the first thing that is "running"
            curr = ready.at(0);
        } else {
            curr = {"invalid", -1000, -1000, -1000, -1000, -1000, -1000, -1000};
        }

        /********BEGIN LIST PROCESSING*******/
        for (int i = 0; i < wc.size(); i ++){

            //temp process to keep track of where we are in the list
            process &temp = wc.at(i);

            //go through and update the block list
            //happens whether the CPU is idle or not
            if (temp.blockTimeTotal >= 0) {
                    temp.blockTimeTotal++;
            }

            //update the turnaround for everything that has arrived
            if(temp.arrivalTime <= time){
                temp.turnaround++;
            }

            //find match to running process and "run" it
            if (temp.name == curr.name) {

                //Assume new process
                if (temp.sliceTime == 0 ) {
                    std::cout << " " << time << "\t" << temp.name << "\t";
                }

                temp.runningTime++;
                temp.sliceTime++;
                temp.totalTime--;

                //assuming only one thing can ever change
                //take the things finished out of the list
                if (temp.totalTime == 0) {
                    std::cout << temp.sliceTime << "\tT\n";
                    //add the turnaround about to be removed to sum for average
                    rrTurnSum += temp.turnaround;
                    ready.erase(ready.begin());
                    wc.erase(wc.begin() + i );

                    //need to move our place in the for loop back one
                    //since the list length is now one shorter
                    i--;
                }

                //If the process is blocked
                else if (temp.runningTime == temp.blockInterval) {
                    std::cout << temp.sliceTime << "\tB\n";
                    temp.runningTime = 0;
                    temp.sliceTime = -1;
                    temp.blockTimeTotal = 0;
                    //remove the thing that has blocked from ready
                    ready.erase(ready.begin());
                }

                //a process has ended its time slice add to the back of list
                else if (temp.sliceTime == timeSlice){
                    //running time back to 0
                    std::cout << temp.sliceTime << "\tS\n";
                    temp.sliceTime = 0;
                    //move the sliced thing to back of lists
                    ready.erase(ready.begin());
                    wc.erase(wc.begin() + i);
                    wc.push_back(temp);
                }
            }
        }
        /********END LIST PROCESSING*********/

        if(idleCount >= 0) idleCount++;
        time++;
    }
    
    double averageT = rrTurnSum / (double) processes.size();
    std::cout << " " << time << "\t<done>\t" << averageT << "\n";
}

