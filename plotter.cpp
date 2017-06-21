#include "plotter.h"

#include <thread>

#include <iostream>
#include <string>
#include <fstream>


#include <sstream>
#include <algorithm>
#include <iterator>

#include "sharingqueues.h"
#include "speedcalculate.h"

using namespace std;



Plotter::Plotter(): state(CONF),dtSim(1),dtLog(1),
    logMap(SHARING_QUEUE::getLogMap())
{
    logMap.insertNewKey("base");
}

void Plotter::addLogThread()
{
    mutex mtx;
    const atomic<bool>& fin = SHARING_QUEUE::getToFisinish();
    const State & s=state;

    auto lfunc = [&s,&fin]{
        bool res = (s == SIM || fin);
        return res;
    };

    while(!fin)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(dtLog*1000)));

        unique_lock<mutex> mlock(mtx);
        logThrWaitCond.wait(mlock, lfunc);

        for(pair<string,Pen> pair : penMap)
        {
            string logStr = pair.second.getLogString();
            ostringstream oss;
            oss << time<<";"<<logStr<<"\n";

            logMap.push(pair.first, oss.str() );
        }
    }
    cout<<"finish addLogTh";
}

void Plotter:: runThread()
{

    atomic<bool>& fin = SHARING_QUEUE::getToFisinish();

    while(!fin)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(dtSim*1000)));

        SynchroQueue<string> &cmdQueue = SHARING_QUEUE::getCmdQueue();

        string str = cmdQueue.pop(); // no wait if empty
        while(!str.empty())
        {
            processCmd(str);
            str = cmdQueue.pop();
        }

        switch (state) {
        case CONF:
            break;
        case SIM:
            sims();
            break;
        case END:
            fin=true;
            break;
        default:
            return;
        }
    }

    cout<<"finish RunThread";
}


void Plotter::processCmd(string str)
{

    if(str.empty()) return;
    istringstream iss(str);
    vector<string> v{istream_iterator<string>{iss},
                     istream_iterator<string>{}};


    str.append("\n");


    logMap.push("base",move(str));

    if(v.front()=="start" && v.size() == 1)
    {
        state = SIM;
        logThrWaitCond.notify_one();
        time = 0;
        return;
    }

    if(v.front()=="stop" && v.size() == 1)
    {
        state = END;
        return;
    }

    if(v.front()=="create"  && v.size() == 3)
    {
        if(v.at(1)=="motor")
        {
            motorMap.insert(pair<string, Motor >(v.at(2), Motor() ) );
        }
        if(v.at(1)=="pen")
        {
            penMap.insert(pair<string, Pen >(v.at(2),Pen()));
            logMap.insertNewKey(v.at(2));
        }
        return;
    }

    if(v.front()=="attach"  && v.size() == 6)
    {
        string motName = v.at(1);
        string penName = v.at(5);
        string axisName= v.at(3);


        if(axisName == "x")
            penMap.at(penName).setMotorX(  & motorMap.at(motName));
        if(axisName == "y")
            penMap.at(penName).setMotorY(  & motorMap.at(motName));

        return;
    }

    if(v.front()=="set")
    {
        if((v.at(1)=="sim" || v.at(1)=="log") && v.size()==3)
        {
            string s = v.at(2).substr(0, v.at(2).find("="));
            if(s =="dT")
            {
                string paramValueStr = v.at(2).substr((v.at(2).find("="))+1);
                if(v.at(1)=="sim")
                    dtSim = stof(paramValueStr);
                else if(v.at(1)=="log")
                    dtLog = stof(paramValueStr);
            }
            return;
        }

        if((v.at(1)=="motor" ) && v.size()==4)
        {
            string motorName = v.at(2);
            string motorParam = v.at(3).substr(0, v.at(3).find("="));

            string strParVal=v.at(3).substr((v.at(3).find("="))+1);
            float paramValue = stof(strParVal);

            if(motorParam == "S")
                motorMap.at(motorName).S_max_aups = paramValue;

            else if(motorParam =="A")
                motorMap.at(motorName).A_aupss = paramValue;

            else if(motorParam =="P")
                motorMap.at(motorName).TP = paramValue;
            return;
        }
        if((v.at(1)=="pen" ) && v.size()==4)
        {
            string penName = v.at(2);
            string value = v.at(3);
            bool resVal;
            if(value == "on")
                resVal=true;
            else if(value == "off")
                resVal=false;
            penMap.at(penName).setDraw(resVal);
            return;
        }
    }
}

void Plotter:: sims()
{
    for(pair<string,Pen> pair : penMap)
    {
        pair.second.step(dtSim);
    }
    time += dtSim;
}
