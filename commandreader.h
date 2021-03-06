#ifndef COMMANDREADER_H
#define COMMANDREADER_H

#include "sharingqueues.h"
#include <atomic>

class CommandReader
{
public:
    CommandReader();
    void run();
private:
    SynchroQueue<string> &cmdQueue;
    const atomic<bool>& fin;
};

#endif // COMMANDREADER_H
