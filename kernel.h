#ifndef KERNEL_H
#define KERNEL_H

#include <thread>


#include "plotter.h"
#include "logwriter.h"
#include "commandreader.h"




class Kernel
{
public:
    explicit Kernel();
    void theadStart();
 private:
    atomic<bool>& fin;
    Plotter plotter;
    LogWriter logWriter;
    CommandReader commandReader;


    Kernel(const Kernel&) = delete;
    Kernel& operator =(const Kernel&) = delete;
};




#endif // KERNEL_H
