#include "logwriter.h"
#include "sharingqueues.h"

LogWriter::LogWriter(): logMap(SHARING_QUEUE::getLogMap()),
                        fin(SHARING_QUEUE::getToFisinish())
{
    logMap.insertNewKey("base");
}


void LogWriter::run()
{
    while(!fin)
    {
        const map<string, queue<string> >& logMapRead = logMap.getMapToRead();//wait while queues are empty
        if(logMapRead.empty())
            continue;
        map <string, queue<string> >::const_iterator  mapEl;
        for(mapEl = logMapRead.begin(); mapEl != logMapRead.end(); ++mapEl)
        {
            if((*mapEl).second.empty())
                continue;
            string nameFile = (*mapEl).first + ".log";
            if( checkFilesMap(nameFile) == ERROR)
                continue;

            while(!(*mapEl).second.empty())
            {
                string str =  (*mapEl).second.front();
                logMap.pop((*mapEl).first);
                *filesMap.at(nameFile).get()<<str;
            }
        }
    }

    closeFileStreams();

    cout<<"fin logWriter";
}



LogWriter::ReturnResult LogWriter::checkFilesMap(string nameFile)
{
    map<string,shared_ptr< ofstream> >::iterator it = filesMap.find(nameFile);
    if(it == filesMap.end())
    {
        shared_ptr<ofstream> streamPtr(new ofstream(nameFile.c_str(), std::ios_base::out));
        if(!streamPtr.get()->is_open())
            return ERROR;

        filesMap[nameFile] = streamPtr;
    }
    return NOERROR;
}


void LogWriter::closeFileStreams()
{
    for (map<string, shared_ptr<ofstream>>::iterator iter = filesMap.begin();
         iter != filesMap.end(); ++iter)
    {
       ofstream * stream = (*iter). second.get();
       if(stream->is_open() )
        stream->close();
    }
}
