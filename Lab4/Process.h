/* 
 * File:   Process.h
 * Authors: Elisabeth Bristol and Levi Muniz
 *
 * Created on February 12, 2019, 4:44 PM
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <MMU.h>

class Process {
public:
    /*Constructor for Process 
     * @param filename - name of the file we are reading from
     * @throws run time error if the file cannot be opened
     */
    Process(const std::string &filename);

    //Process Destructor that closes the file and deals with memory leaks
    ~Process();

    //rule of 5 explicit deletes
    Process(const Process& other) = delete;
    Process(Process&& other) = delete;
    Process& operator=(const Process& other) = delete;
    Process& operator=(Process&& other) = delete;

    /* Exec function that acts as a parser for commands in the file
     * @return void
     */
    void Exec();

private:
    std::fstream *file;
    std::uint64_t line;
    mem::MMU *mem;

    /* memsize - addresses behavior when encountering command memsize
     * @param address - refers to the size of address space to set vector
     * @return void
     */
    void memsize(int address);

    /*cmp - compares a number of bytes at two given addresses
     * @param address1 - first address in comparison
     * @param address2 - second address in comparison
     * @param count - number of bytes to be compared
     * @throws - standard error with message if the compared things don't match
     * @return void
     */
    void cmp(int address1, int address2, int count);

    /*set - takes a list of values and sets them in memory from an address
     * @param address - the starting spot in memory
     * @param v - a vector containing the list of values to add
     * @return - void
     */
    void set(int address, std::vector<std::uint8_t> &v);

    /* fill - puts count number of some value starting at an address
     * @param address - the starting address
     * @param value - the value we will use for the filling
     * @param count - the number of times we add that value
     * @return - void
     */
    void fill(int address, int value, int count);

    /*dup - copies a number of bytes starting at one location to new
     * @param src_address - the starting location for the copy
     * @param dest_address -  the destination of the copy
     * @param count - the number of bytes to be duplicated
     * @return - void
     */
    void dup(int src_address, int dest_address, int count);

    /* print - prints the status of memory in 16 byte intervals 
     * displays the address of each 16 byte chunks at the beginning of each row
     * @param address -  the starting point the print comes from
     * @param count - the number of bytes we are displaying
     * @return - void
     */
    void print(int address, int count);
};

#endif /* PROCESS_H */
