#include "pen.h"

#include <sstream>

Pen::Pen():xM(nullptr),yM(nullptr),draw(true)
{

}

void Pen::step(float dt)
{
    if(xM)
       xM.load()->step(dt);

    if(yM)
       yM.load()->step(dt);

}

string Pen::getLogString() const
{
    string logStr;
    ostringstream logOss;

    if(!draw) return "";

    if(xM)
        logOss<<xM.load()->getCurrentPosition()<<";";

    if(yM )
        logOss<<yM.load()->getCurrentPosition();

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


//bool Pen::isDraw() const
//{
//    return draw;
//}

void Pen::setDraw(bool draw_)
{
 draw = draw_;
}
