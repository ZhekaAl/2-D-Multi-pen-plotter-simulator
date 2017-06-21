#include "pen.h"

#include <sstream>

Pen::Pen():xM(nullptr),yM(nullptr),draw(true)
{

}

void Pen::step(float dt)
{
    if(xM)
     xM->step(dt);
    if(yM)
     yM->step(dt);
}

string Pen::getLogString() const
{
    string logStr;
    ostringstream logOss;

    if(xM && draw)
        logOss<<xM->P<<";";
    else
        logOss<<"0;";

    if(yM && draw)
            logOss<<yM->P;
        else
            logOss<<"0";

    logStr = logOss.str();

    return logStr;
}

void Pen::setMotorX(Motor* motPtr)
{
    if(motPtr!= nullptr)
     xM = motPtr;
}

void Pen::setMotorY(Motor* motPtr)
{
    if(motPtr!= nullptr)
     yM = motPtr;
}


bool Pen::isDraw() const
{
    return draw;
}

void Pen::setDraw(bool draw_)
{
 draw = draw_;
}
