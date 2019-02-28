
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

//extends the MMU's fault handler to print appropriate messages
class WriteFaultHandler: public mem::MMU::FaultHandler {
public:
    WriteFaultHandler(){}
    
    //explicit deletes
    WriteFaultHandler(const WriteFaultHandler& other) = delete;
    WriteFaultHandler(WriteFaultHandler&& other) = delete;
    WriteFaultHandler& operator=(const WriteFaultHandler& other) = delete;
    WriteFaultHandler& operator=(WriteFaultHandler&& other) = delete;

    ~WriteFaultHandler() = default;
    
    /**
     * Run - will print out the appropriate error message
     * @param pmcb  - the process pmcb where the fault was experienced
     * @return bool- will always return false - used for the errrors
     */
    virtual bool Run(const mem::PMCB &pmcb) {
        std::cout << "Write Permission Fault at address " << std::setfill('0') 
                << std::setw(7) << std::hex << pmcb.next_vaddress << '\n';
        
        return false;
    }
};

class PageFaultHandler: public mem::MMU::FaultHandler {
public:
    PageFaultHandler(){}
    
    //explicit deletes
    PageFaultHandler(const PageFaultHandler& other) = delete;
    PageFaultHandler(PageFaultHandler&& other) = delete;
    PageFaultHandler& operator=(const PageFaultHandler& other) = delete;
    PageFaultHandler& operator=(PageFaultHandler&& other) = delete;

    ~PageFaultHandler() = default;
    
    /**
     * run - will print out the appropriate error message for 2 different 
     * kinds of page faults. 
     * @param pmcb - the pmcb of the process where the fault happened
     * @return 
     */
    virtual bool Run(const mem::PMCB &pmcb) {
        
        //read page fault
        if(pmcb.operation_state == mem::PMCB::READ_OP){
            std::cout << "Read Page Fault at address " << std::hex
                    << std::setfill('0') << std::setw(7) 
                    << pmcb.next_vaddress << '\n';
        } 
        //write page fault
        else if (pmcb.operation_state == mem::PMCB::WRITE_OP){
            std::cout << "Write Page Fault at address " << std::hex
                    << std::setfill('0') << std::setw(7)
                    << pmcb.next_vaddress << '\n';
        } 
        
        else {
            
        }
        
        return false;
    }
};

#endif /* FAULTHANDLER_H */

