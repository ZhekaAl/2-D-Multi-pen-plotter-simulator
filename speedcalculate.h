#ifndef SPEEDCALCULATE_H
#define SPEEDCALCULATE_H

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

#endif // SPEEDCALCULATE_H
