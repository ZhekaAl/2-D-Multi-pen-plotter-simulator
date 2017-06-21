#ifndef KERNEL_H
#define KERNEL_H

#include <thread>


#include "plotter.h"
#include "logwriter.h"
#include "commandreader.h"




class Kernel
{
public:
    Kernel();
    void theadStart();
 private:
    atomic<bool>& fin;
    Plotter plotter;
    LogWriter logWriter;
    CommandReader commandReader;
};




#endif // KERNEL_H
