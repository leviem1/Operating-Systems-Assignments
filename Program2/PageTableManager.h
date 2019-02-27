
/* 
 * File:   PageTableManager.h
 * Author: Elisabeth Bristol and Levi Muniz
 *
 * Created on February 25, 2019, 5:19 PM
 */

#ifndef PAGETABLEMANAGER_H
#define PAGETABLEMANAGER_H

class PageTableManager {
public:
    
    PageTableManager();
  
    PageTableManager(const PageTableManager &other) = delete;  // no copy constructor
    PageTableManager(PageTableManager &&other) = delete;       // no move constructor
    PageTableManager operator=(const PageTableManager &other) = delete;  // no copy assign
    PageTableManager operator=(PageTableManager &&other) = delete;       // no move assign
   
    ~PageTableManager();
private:

};

#endif /* PAGETABLEMANAGER_H */

