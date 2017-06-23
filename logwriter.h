#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <fstream>
#include <iostream>
#include <atomic>


#include "synchrocontainer.h"


using namespace std;

class LogWriter
{
   enum ReturnResult{NOERROR, ERROR};

   public:

    explicit LogWriter();
    void run();

   private:

    ReturnResult checkFilesMap(string nameFile);
    void closeFileStreams();

    map<string, shared_ptr<ofstream> > filesMap;
    SynchroMap<string>& logMap;
    const atomic<bool>& fin;

    LogWriter(const LogWriter&) = delete;
    LogWriter& operator =(const LogWriter&) = delete;
};

#endif // LOGWRITER_H
