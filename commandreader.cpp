#include "commandreader.h"
#include <iostream>
#include <sstream>



CommandReader::CommandReader():
    cmdQueue(SHARING_QUEUE::getCmdQueue()),
    fin(SHARING_QUEUE::getToFisinish())
{

}

void CommandReader::run()
{
    while (!fin && std::cin) {

        std::string s;
        std::getline(cin,s);
        cmdQueue.push(s);

        if(s == "stop")
            break;

    }
    cout<<"finCmdReader;";
}
