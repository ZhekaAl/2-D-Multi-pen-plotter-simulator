#include <iostream>
#include <thread>

using namespace std;


#include "plotter.h"


int main()
{
    cout << "Hello World!" << endl;

    Plotter plotter;
    theadStart(&plotter);
    // theadStart();

    return 0;
}

