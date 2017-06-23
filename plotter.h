#ifndef PLOTTER_H
#define PLOTTER_H

#include <ostream>
#include <queue>
#include <string>
#include <map>
#include <atomic>

#include "synchrocontainer.h"
#include "motor.h"
#include "pen.h"


using namespace std;


class Plotter
{
    enum State {CONF, SIM, END};

public:

    explicit Plotter();
    void runThread();
    void addLogThread();

private:

    void sims();
    void processCmd(string str);

    map<string, Pen> penMap;
    map<string, Motor> motorMap;
    State state;

    float time; // current time simulation
    float dtSim; //stepUpdateStates
    float dtLog; //stepToWriteLog

    SynchroMap<string>& logMap; //reference! to shared data

    condition_variable logThrWaitCond;// for wait adLog Thread, when state != SIMS

    Plotter& operator =(const Plotter&)=delete;
    Plotter(const Plotter&) = delete;

};






#endif // PLOTTER_H
