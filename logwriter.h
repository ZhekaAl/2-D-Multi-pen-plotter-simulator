#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <fstream>
#include <iostream>
#include <memory>


#include "threadqueue.h"
#include "atomic"

using namespace std;

class LogWriter
{
   enum ReturnResult{NOERROR, ERROR};

   public:

    LogWriter();
    void run();

   private:

    ReturnResult checkFilesMap(string nameFile);
    void closeFileStreams();

    map<string, shared_ptr<ofstream> > filesMap;
    ThreadMap<string>& logMap;
    const atomic<bool>& fin;
};

#endif // LOGWRITER_H
