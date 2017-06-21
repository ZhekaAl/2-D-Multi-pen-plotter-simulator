#include "staticimitationinputcommand.h"



void initScen()
{
    ThreadQueue<string> &cmdQueue  =  SHARING_QUEUE::getCmdQueue();

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
