#ifndef MOTOR_H
#define MOTOR_H

#include "speedcalculate.h"

#include <mutex>



class Motor
{
public:

    explicit Motor();
    float getCurrentPosition() const;
    void setTargetPosition(float);
    void setMaxSpeed(float);
    void setMaxAcceleration(float);
    void step(float dt);

private:

    void calculate(float dt);
    float maxSpeed;//max speed
    float maxAcceleration;//fixed absolute value acceleration( > 0), in real process  acceleration can be -A_aupss,+A_aupss or 0
    float currentAcceleration; //current acceleration
    float targetPosition;//target position
    float currentPosition;//current position
    float currentSpeed;//current speed

    Motor&  operator=(const Motor& )= delete;

    mutable std::mutex m_mutex;

};

#endif // MOTOR_H
