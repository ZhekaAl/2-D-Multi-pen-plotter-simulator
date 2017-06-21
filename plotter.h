#ifndef PLOTTER_H
#define PLOTTER_H

#include <ostream>
#include <queue>
#include <string>
#include <map>
#include <atomic>

#include "threadqueue.h"
#include "motor.h"
#include "pen.h"


using namespace std;


class Plotter
{
    enum State {CONF, SIM, END};

public:

    Plotter();
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

    ThreadMap<string>& logMap; //reference! to shared data

    condition_variable logThrWaitCond;// for wait adLog Thread, when state != SIMS

};






#endif // PLOTTER_H
