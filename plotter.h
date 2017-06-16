#ifndef PLOTTER_H
#define PLOTTER_H


#include <ostream>
#include <queue>
#include  <string>
#include <map>

using namespace std;


namespace THREADS_QUEUE
{
    static queue<string>& getCmdQueue(){
        static queue<string> cmdQueue;
        return cmdQueue;
    }

    static map<string, queue<string> >& getLogMap(){
        static map<string, queue<string> > logMap;
        return logMap;
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
    void calculate();
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

    void theadStart();
private:

    void run();

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

SpeedChange nextSpeedChange(float V0, float x0, float a, float Vmax, float xT);

}

#endif // PLOTTER_H
