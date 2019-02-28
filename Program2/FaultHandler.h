/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FaultHandler.h
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on February 27, 2019, 7:01 PM
 */

#ifndef FAULTHANDLER_H
#define FAULTHANDLER_H

#include <MMU.h>
#include <iostream>

class WriteFaultHandler: public mem::MMU::FaultHandler {
public:
    WriteFaultHandler(){}
    
    WriteFaultHandler(const WriteFaultHandler& other) = delete;
    WriteFaultHandler(WriteFaultHandler&& other) = delete;
    WriteFaultHandler& operator=(const WriteFaultHandler& other) = delete;
    WriteFaultHandler& operator=(WriteFaultHandler&& other) = delete;

    ~WriteFaultHandler() = default;
    
    virtual bool Run(const mem::PMCB &pmcb) {
        std::cout << "Write Permission Fault at address " << std::setfill('0') 
                << std::setw(7) << std::hex << pmcb.next_vaddress << '\n';
        
        return false;
    }
    
    
private:
    
};

class PageFaultHandler: public mem::MMU::FaultHandler {
public:
    PageFaultHandler(){}
    
    PageFaultHandler(const PageFaultHandler& other) = delete;
    PageFaultHandler(PageFaultHandler&& other) = delete;
    PageFaultHandler& operator=(const PageFaultHandler& other) = delete;
    PageFaultHandler& operator=(PageFaultHandler&& other) = delete;

    ~PageFaultHandler() = default;
    
    virtual bool Run(const mem::PMCB &pmcb) {
        
        if(pmcb.operation_state == mem::PMCB::READ_OP){
            std::cout << "Read Page Fault at address " << std::hex
                    << std::setfill('0') << std::setw(7) 
                    << pmcb.next_vaddress << '\n';
        } 
        
        else if (pmcb.operation_state == mem::PMCB::WRITE_OP){
            std::cout << "Write Page Fault at address " << std::hex
                    << std::setfill('0') << std::setw(7)
                    << pmcb.next_vaddress << '\n';
        } 
        
        else {
            
        }
        
        return false;
    }
    
    
private:
    
};

#endif /* FAULTHANDLER_H */

