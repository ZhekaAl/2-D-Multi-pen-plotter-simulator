#include "kernel.h"
#include "sharingqueues.h"

#include "staticimitationinputcommand.h"

Kernel::Kernel(): fin(SHARING_QUEUE::getToFisinish())
{

}

void Kernel::theadStart()
{

    fin = false;

    //initScen();// imit cmd input

    thread commandReaderThread(&CommandReader::run, &commandReader);
    thread plotterRunThread(&Plotter::runThread,&plotter);
    thread plotterAddLogThread(&Plotter::addLogThread,&plotter);
    thread logWriterThread(&LogWriter::run, &logWriter);

    commandReaderThread.join();
    plotterRunThread.join();
    plotterAddLogThread.join();
    logWriterThread.join();

    cout<<"allFinished";

}
