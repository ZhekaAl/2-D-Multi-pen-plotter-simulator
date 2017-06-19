#ifndef PLOTTER_H
#define PLOTTER_H


#include <ostream>
#include <queue>
#include  <string>
#include <map>
#include <atomic>

#include "threadqueue.h"

using namespace std;


namespace THREADS_QUEUE
{

static ThreadQueue<string>& getCmdQueue(){
        static ThreadQueue<string> cmdQueue;
        return cmdQueue;
    }


    static ThreadMap<string>& getLogMap(){
        static ThreadMap<string> logMap;
        return logMap;
    }

   static atomic<bool>& getToFisinish(){
      static atomic<bool> finishAll;
      return finishAll;
   }

}


class Motor
{
public:

    Motor();

    float S_max_aups;//max speed
    float A_aupss;//fixed absolute value acceleration( > 0), in real process  acceleration can be -A_aupss,+A_aupss or 0
    float A; //current acceleration
    float TP;
    float P;
    float V;

    void step(float dt);
    void calculate(float dt);
};

class Pen
{
public:
    Pen();
    void step(float dt);
    string getLogString() const;

    void setMotorX(Motor* motPtr);
    void setMotorY(Motor* motPtr);

    bool isDraw() const;
    void setDraw(bool draw_);

private:
    Motor* xM;
    Motor* yM;
   bool draw;
};




class Plotter
{

  enum State {CONF, SIM, END};
public:
    Plotter();
    void runThread(); //
    void addLogThread();

private:
    void conf();
    void sims();
    void end();

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

//a- abs value
namespace TRAEKT
{
enum SpeedChange
{
    INCREASE,
    DECREASE,
    NOCHANGE,
    ERROR
};

SpeedChange nextSpeedChange(float V0, float x0, float a, float xT, float dt);

}

void initScen();


void cmdReader();
void logWriter();
void initBaseLog();

void theadStart(Plotter* plotter);







#endif // PLOTTER_H
