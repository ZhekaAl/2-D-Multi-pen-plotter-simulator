#include "speedcalculate.h"


#include <algorithm>

#include "sharingqueues.h" //----only for out debug in log file
#include <sstream>         //----

TRAEKT::SpeedChange TRAEKT::nextSpeedChange(float Vc, float x0, float a, float xT, float dt)
{
   using namespace TRAEKT;
   SpeedChange res = ERROR;
   if(a <= 0 || dt <= 0) return  res;

   float xDown = 0.5 * (Vc/(a*dt) +1) * Vc * dt ;// distance to down from current speed to 0

   float ds = abs(xT - x0); //distance to target

/*1 near target*/ if(ds < a*dt*dt)
                   {
                    if( abs(Vc) < a*dt) res = NOCHANGE;
                    else if( Vc > 0) res = DECREASE;
                    else if( Vc < 0) res = INCREASE;
                   }

/*2 wrong direction*/else if(x0 < xT && Vc <= 0) res = INCREASE;
                      else if(x0 > xT && Vc >= 0) res = DECREASE;

/*3  general       */else if(ds < xDown){
                        if(x0 < xT) res = DECREASE;
                        else if(x0 > xT) res = INCREASE;
                     }
                     else if(abs(ds - xDown) < a*dt)  res = NOCHANGE;

                     else if(ds > xDown){
                        if(x0 < xT)  res = INCREASE;
                        if(x0 > xT)  res = DECREASE;
                    }

    //------for debug trace
        float d = a*dt; float Vcm = 0;//v max in current time, such that v can be 0, when x0=xT
        Vcm = -d + sqrt(pow(d,2)+8*d*(xT-x0));
        ostringstream ossLog;
        ossLog<<"nextSpeedChange Vc="<<Vc<<" a="<<a<<" dt="<<dt<<" xt-x0="<<xT-x0<<" xDown="<<xDown<<" Vcm="<<Vcm<<" res=(0+,1-,2=)"<<res<<endl;
       ThreadMap<string>& mapLog = SHARING_QUEUE::getLogMap();
       mapLog.push("base",ossLog.str()) ;
       // logMap.
    //------end

    return res;
}

