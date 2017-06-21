#include "motor.h"

#include "algorithm"

using namespace std;

Motor::Motor():S_max_aups(1),
               A_aupss(1),
               A(0),
               P(0),
               V(0)
{

}

void Motor::step(float dt)
{
    P = P + V*dt;

    calculate(dt);//change A

    V = min(V + A*dt, S_max_aups);
}

void Motor::calculate(float dt)
{
   using namespace TRAEKT;

  SpeedChange change =
          nextSpeedChange(V, P, A_aupss, TP, dt);

  switch(change)
  {
  case INCREASE :
      A = abs(A_aupss);
      break;
  case DECREASE :
      A = -abs(A_aupss);
      break;
  case NOCHANGE:
      A = 0;
      break;
  default: break;
  }
}
