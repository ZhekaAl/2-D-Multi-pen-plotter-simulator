#include "plotter.h"
#include <thread>

#include <iostream>
#include <string>
#include <fstream>


#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;


void cmdReader()
{

    ThreadQueue<string> &cmdQueue  =  THREADS_QUEUE::getCmdQueue();
    const atomic<bool>& fin = THREADS_QUEUE::getToFisinish();

    while (!fin && std::cin) {

        string s;
        getline(cin,s);
        cmdQueue.push(s);
        if(s == "stop")
            break;

    }
    cout<<"finCmdReader;";
}


void logWriter()
{

   ThreadMap<string>& logMap = THREADS_QUEUE::getLogMap();
   initBaseLog();
   const atomic<bool>& fin = THREADS_QUEUE::getToFisinish();

   while( !fin)
      {
      const map<string, queue<string> >& logMapRead = logMap.getMapToRead();//wait while queues are empty
      map <string, queue<string> >::const_iterator  mapEl;
      for( mapEl=  logMapRead.begin();mapEl!=logMapRead.end(); ++mapEl)
      {
          if((*mapEl).second.empty())
              continue;
          string nameFile = (*mapEl).first + ".log";
          std::ofstream myfile;
          myfile.open(nameFile.c_str(),std::ios_base::app | std::ios_base::out);
          if(!myfile.is_open())
              continue;

          while(!(*mapEl).second.empty())
          {
           string str =  std::move((*mapEl).second.front());
           logMap.pop((*mapEl).first);
           myfile<<std::move(str);
          }
         if(myfile.is_open()) myfile.close();
      }

    }

   cout<<"fin logWriter";


}


void initScen()
{
    ThreadQueue<string> &cmdQueue  =  THREADS_QUEUE::getCmdQueue();

    string s = "create pen p1";
    cmdQueue.push(std::move(s));

    s = "create motor m1";
    cmdQueue.push(std::move(s));

    s = "create motor m2";
    cmdQueue.push(std::move(s));

    s = "attach m1 with x of p1";
    cmdQueue.push(std::move(s));

    s = "attach m2 with y of p1";
    cmdQueue.push(std::move(s));

    s = "set motor m1 S=40";
    cmdQueue.push(std::move(s));

    s = "set motor m2 S=40";
    cmdQueue.push(std::move(s));

    s = "set motor m1 A=4";
    cmdQueue.push(std::move(s));

    s = "set motor m2 A=2";
    cmdQueue.push(std::move(s));

    s = "set motor m1 P=200";
    cmdQueue.push(std::move(s));

    s = "set motor m2 P=120";
    cmdQueue.push(std::move(s));

    s = "create pen p2";
        cmdQueue.push(std::move(s));

        s = "create motor m3";
        cmdQueue.push(std::move(s));

        s = "create motor m4";
        cmdQueue.push(std::move(s));

        s = "attach m3 with x of p2";
        cmdQueue.push(std::move(s));

        s = "attach m4 with y of p2";
        cmdQueue.push(std::move(s));

        s = "set motor m3 S=20";
        cmdQueue.push(std::move(s));

        s = "set motor m4 S=20";
        cmdQueue.push(std::move(s));

        s = "set motor m3 A=4";
        cmdQueue.push(std::move(s));

        s = "set motor m4 A=3";
        cmdQueue.push(std::move(s));

        s = "set motor m3 P=300";
        cmdQueue.push(std::move(s));

        s = "set motor m4 P=220";
        cmdQueue.push(std::move(s));


    s = "set sim dT=0.1";
    cmdQueue.push(std::move(s));

  //  s = "start";
   // cmdQueue.push(std::move(s));

}

void initBaseLog()
{
    std::remove("base.log");
    std::remove("p1.log");
    std::remove("p2.log");

    ThreadMap<string>& logMap = THREADS_QUEUE::getLogMap();
    logMap.insertNewKey("base");
}

Plotter::Plotter(): state(CONF),dtSim(1),dtLog(1),
                    logMap(THREADS_QUEUE::getLogMap())
{

}


void theadStart(Plotter * plotter)
{
    atomic<bool>& fin = THREADS_QUEUE::getToFisinish();
    fin = false;

    initScen();// imit cmd input

    thread t1(&Plotter::runThread,plotter);
    thread r(cmdReader);
    thread w1(&Plotter::addLogThread,plotter);
    thread w2(logWriter);

    t1.join();
    r.join();
    w1.join();
    w2.join();

    cout<<"allFinished";

}


void Plotter::addLogThread()
{

    mutex mtx;
    //condition_variable cond;

    const atomic<bool>& fin = THREADS_QUEUE::getToFisinish();
    const State & s=state;

    auto lfunc = [&s,&fin]{
        bool res = (s == SIM || fin);
        return res;
    };

    while(!fin)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(dtLog*1000)));


        unique_lock<mutex> mlock(mtx);
        logThrWaitCond.wait(mlock, lfunc);

        for(pair<string,Pen> pair : penMap)
        {
            string logStr = pair.second.getLogString();
            ostringstream oss;
            oss << time<<";"<<logStr<<"\n";

            logMap.push(pair.first, oss.str() );
        }
    }
    cout<<"finish addLogTh";
}

void Plotter:: runThread()
{

   atomic<bool>& fin = THREADS_QUEUE::getToFisinish();

  while(!fin)
  {
   std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(dtSim*1000)));

   ThreadQueue<string> &cmdQueue = THREADS_QUEUE::getCmdQueue();

   string str = cmdQueue.pop(); // no wait if empty
   while(!str.empty())
   {
      processCmd(str);
      str = cmdQueue.pop();
   }

   switch (state) {
   case CONF:
       conf();
       break;
   case SIM:
       sims();
       break;
   case END:
       fin=true;
       break;
   default:
       return;
   }
   }

  cout<<"finish RunThread";
}



void Plotter::processCmd(string str)
{

    if(str.empty()) return;
    istringstream iss(str);
    vector<string> v{istream_iterator<string>{iss},
                          istream_iterator<string>{}};


    str.append("\n");


    logMap.push("base",move(str));

    if(v.front()=="start" && v.size() == 1)
    {
        state = SIM;
        logThrWaitCond.notify_one();
        time = 0;
        return;
    }

    if(v.front()=="stop" && v.size() == 1)
    {
        state = END;

        return;
    }

    if(v.front()=="create"  && v.size() == 3)
    {
        if(v.at(1)=="motor")
        {
            motorMap.insert(pair<string, Motor >(v.at(2), Motor() ) );
        }
        if(v.at(1)=="pen")
        {
            penMap.insert(pair<string, Pen >(v.at(2),Pen()));
            logMap.insertNewKey(v.at(2));
        }
        return;
    }

    if(v.front()=="attach"  && v.size() == 6)
    {
        string motName = v.at(1);
        string penName = v.at(5);
        string axisName= v.at(3);


        if(axisName == "x")
        //penMap.at(penName).xM = & motorMap.at(motName);
            penMap.at(penName).setMotorX(  & motorMap.at(motName));
        if(axisName == "y")
            //penMap.at(penName).yM = & motorMap.at(motName);
            penMap.at(penName).setMotorY(  & motorMap.at(motName));

    return;
    }

    if(v.front()=="set")
    {
        if((v.at(1)=="sim" || v.at(1)=="log") && v.size()==3)
        {

           //vector<string> v1= split(v.at(2), "=");
           string s = v.at(2).substr(0, v.at(2).find("="));

           if(s =="dT")
           {
               string paramValueStr = v.at(2).substr((v.at(2).find("="))+1);
               if(v.at(1)=="sim")
                dtSim = stof(paramValueStr);
               else if(v.at(1)=="log")
                dtLog = stof(paramValueStr);
           }

           return;
        }

        if((v.at(1)=="motor" ) && v.size()==4)
        {
            string motorName = v.at(2);
            string motorParam = v.at(3).substr(0, v.at(3).find("="));

          //  float data[4];
            string strParVal=v.at(3).substr((v.at(3).find("="))+1);
            float paramValue = stof(strParVal);

          //  float paramValue = data[0];

            if(motorParam == "S")
                motorMap.at(motorName).S_max_aups = paramValue;

            else if(motorParam =="A")
                motorMap.at(motorName).A_aupss = paramValue;

            else if(motorParam =="P")
                motorMap.at(motorName).TP = paramValue;
        return;
        }
        if((v.at(1)=="pen" ) && v.size()==4)
        {
           string penName = v.at(2);
           string value = v.at(3);
           bool resVal;
          if(value == "on")
             resVal=true;
          else if(value == "off")
              resVal=false;
          penMap.at(penName).setDraw(resVal);
          return;
        }
   }
}

bool Pen::isDraw() const
{
    return draw;
}

void Pen::setDraw(bool draw_)
{
 draw = draw_;
}



void Plotter:: conf()
{

}


void Plotter:: sims()
{
    for(pair<string,Pen> pair : penMap)
    {
        pair.second.step(dtSim);
    }
    time += dtSim;
}

void Plotter:: end()
{
  //cout<<"end";
}


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
       ThreadMap<string>& mapLog = THREADS_QUEUE::getLogMap();
       mapLog.push("base",ossLog.str()) ;
       // logMap.
    //------end

    return res;
}


