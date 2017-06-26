#ifndef PEN_H
#define PEN_H

#include<string>
#include <atomic>


#include"motor.h"


using namespace std;

class Pen
{
public:
    explicit Pen();
    void step(float dt);
    std::string getLogString() const;

    void setMotorX(Motor* motPtr);
    void setMotorY(Motor* motPtr);

  //  bool isDraw() const;
    void setDraw(bool draw_);

private:
    atomic<Motor*> xM;
    atomic<Motor*> yM;
    atomic_bool draw;

    Pen&  operator=(const Pen& )= delete;

};

#endif // PEN_H
