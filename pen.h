#ifndef PEN_H
#define PEN_H

#include<string>

#include"motor.h"

using namespace std;

class Pen
{
public:
    Pen();
    void step(float dt);
    std::string getLogString() const;

    void setMotorX(Motor* motPtr);
    void setMotorY(Motor* motPtr);

    bool isDraw() const;
    void setDraw(bool draw_);

private:
    Motor* xM;
    Motor* yM;
    bool draw;
};

#endif // PEN_H
