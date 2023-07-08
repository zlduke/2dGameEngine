#include <iostream>
#include "practice.h"

int main(int argc, char *argv[])
{
    IComp ic0;
    Comp<int> c0;
    c0.ShowId();
    c0.GetId();
    c0.ShowId();
    Comp<float> c1;
    c0.ShowId();
    c1.ShowId();

    return 0;
}
