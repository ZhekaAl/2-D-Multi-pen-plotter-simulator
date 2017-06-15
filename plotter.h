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

    Motor():S_max_aups(1),A_aupss(1),P(0),V(0){}

    float S_max_aups;//max V
    float A_aupss;//a
    int TP;
    int P;
    float V;

    void step(float dt);
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

    map<string, Pen> penMap;
    map<string, Motor> motorMap;
    State state;

    void run();



    void conf();
    void sims();
    void end();

    void processCmd(string str);

    float time;

    float dtSim;
    float dtLog;


};

#endif // PLOTTER_H
