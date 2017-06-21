#ifndef MOTOR_H
#define MOTOR_H

#include "speedcalculate.h"

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

#endif // MOTOR_H
