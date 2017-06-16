#include "plotter.h"
#include <thread>

#include <iostream>
#include <string>
#include <fstream>


#include <sstream>
#include <algorithm>
#include <iterator>


using namespace std;




void Reader()
{
    while (std::cin) {

        auto s = std::string();
        getline(cin,s);

       // auto lock = std::unique_lock<std::mutex>(m);

        queue<string> &que  =  THREADS_QUEUE::getCmdQueue();
        que.push(std::move(s));

        //sAvail.notify_all();
    }
}

void Writer()
{
    while(1)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));

      map<string, queue<string> >& logMap = THREADS_QUEUE::getLogMap();


      map <string, queue<string> >::iterator mapEl;
      for( mapEl=  logMap.begin();mapEl!=logMap.end(); ++mapEl)
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
           (*mapEl).second.pop();
           myfile<<std::move(str);
          }
         if(myfile.is_open()) myfile.close();
      }

    }
}





void initScen()
{

    queue<string> &que  =  THREADS_QUEUE::getCmdQueue();

    string s = "create pen p1";
    que.push(std::move(s));

    s = "create motor m1";
    que.push(std::move(s));

    s = "attach m1 with x of p1";
    que.push(std::move(s));

    s = "set motor m1 S=10";
    que.push(std::move(s));

    s = "set motor m1 A=1";
    que.push(std::move(s));

    s = "set motor m1 P=100";
    que.push(std::move(s));

    s = "start";
    que.push(std::move(s));

}

void initBaseLog()
{
    std::remove("base.log");
    std::remove("p1.log");
    map<string, queue<string> >& logMap = THREADS_QUEUE::getLogMap();
    logMap.insert(make_pair("base",queue<string>()));
}

Plotter::Plotter(): state(CONF)
{

}

void Plotter::theadStart()
{


    initScen();//!!!

    std::thread t1(&Plotter::run, this);
    std::thread r(Reader);
    std::thread w(Writer);

    t1.join();
    r.join();
    w.join();

}



void Plotter:: run()
{


  initBaseLog();
  while(1)
  {
   std::this_thread::sleep_for(std::chrono::seconds(1));

   queue<string> &cmdQue = THREADS_QUEUE::getCmdQueue();
   //&cmdQue = que_glob;

   while(!cmdQue.empty())
   {

   string str =   std::move(cmdQue.front());
   cmdQue.pop();

   processCmd(str);
    }


   switch (state) {
   case CONF:
       conf();
       break;
   case SIM:
       sims();
       break;
   case END:
       end();
       return;
   default:
       return;
   }
   }
}



void Plotter::processCmd(string str)
{

    if(str.empty()) return;
    istringstream iss(str);
    vector<string> v{istream_iterator<string>{iss},
                          istream_iterator<string>{}};


    str.append("\n");
    map<string, queue<string>>& logMap = THREADS_QUEUE::getLogMap();

    logMap.at("base").push(str);

    if(v.front()=="start" && v.size() == 1)
    {
        state = SIM;
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
             map<string, queue<string> >& logMap = THREADS_QUEUE::getLogMap();
             queue<string> que;
             logMap.insert(make_pair(v.at(2), que));
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
               if(v.at(1)=="sim")
                dtSim = stof(v.at(2));
               else if(v.at(1)=="log")
                dtLog = stof(v.at(2));
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

            if(motorParam =="S")
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
          pair.second.step(1);


          string logStr = pair.second.getLogString();
          ostringstream oss;
          oss << time<<";"<<logStr<<"\n";

          logStr = oss.str();
          map<string, queue<string> >& logMap = THREADS_QUEUE::getLogMap();
          logMap.at(pair.first).push(move(logStr));
      }

    time+=1;
}

void Plotter:: end()
{
  cout<<"end";
}


void Motor::step(float dt)
{

    P = P + V*dt;
    V = min(V + A*dt, S_max_aups);
    calculate();

}

void Motor::calculate()
{
   using namespace TRAEKT;

  SpeedChange change =
          nextSpeedChange(V, P, A_aupss, S_max_aups, TP);

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

Pen::Pen():xM(nullptr),yM(nullptr)
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



TRAEKT::SpeedChange TRAEKT::nextSpeedChange(float Vc, float x0, float a, float Vm, float xT)
{
    using namespace TRAEKT;

    if(xT == x0 && Vc == 0)
            return NOCHANGE;

    //float xDown = Vm*Vm/(2*a)+1;// s down from Vmax to 0

    float xDown = Vm*(Vm+1)/(2.0*a) +1;// s down from Vmax to 0

    if(xT - x0 > xDown)
    {
        if(Vc < Vm)
            return INCREASE;
        if(Vc == Vm)
            return NOCHANGE;
    }
    if(xT - x0 <= xDown)
    {
        //float Vm2 = sqrt(2*a*(xT - x0))-1; //Vmax for possible down V to 0

//        float Vm2 = 2*(xT - x0)-1;
//        if(Vc < Vm2)
//            return INCREASE;
//        //if(Vc == Vm2)
//          //  return NOCHANGE;
//        if(Vc >= Vm2)
//           return DECREASE;

        if(xT - x0 > 0 && Vc > 0)
        {
            return DECREASE;
        }

        if(xT - x0 > 0 && Vc <= 0)
        {
            return INCREASE;
        }


    }


    return ERROR;
}


