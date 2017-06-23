#include "motor.h"

#include "algorithm"

using namespace std;

Motor::Motor():maxSpeed(1),
               maxAcceleration(1),
               currentAcceleration(0),
               currentPosition(0),
               currentSpeed(0)
{

}

void Motor::step(float dt)
{
    currentPosition += currentSpeed*dt;

    calculate(dt);//change A

    currentSpeed = min(currentSpeed + currentAcceleration*dt, maxSpeed);
}

void Motor::calculate(float dt)
{
   using namespace TRAEKT;

  SpeedChange change =
          nextSpeedChange(currentSpeed, currentPosition, maxAcceleration, targetPosition, dt);

  switch(change)
  {
  case INCREASE :
      currentAcceleration = maxAcceleration;
      break;
  case DECREASE :
      currentAcceleration = -maxAcceleration;
      break;
  case NOCHANGE:
      currentAcceleration = 0;
      break;
  default: break;
  }
}

float Motor::getCurrentPosition() const
{
    return currentPosition;
}

void Motor::setTargetPosition(float _targetPosition)
{
     targetPosition = _targetPosition;
}

void Motor::setMaxSpeed(float _maxSpeed)
{
    maxSpeed = _maxSpeed;
}

void Motor::setMaxAcceleration(float _maxAcceleration)
{
    maxAcceleration = _maxAcceleration;
}


