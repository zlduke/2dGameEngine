#ifndef PRACTICE_H
#define PRACTICE_H

#include <iostream>

struct IComp
{
public:
    static int nextId;
    void ShowId()
    {
        std::cout << "ICompId  " << nextId << std::endl;
    }
};

template <typename T>
class Comp : IComp
{
public:
    // this ensures that the func can be called without instantiation of objects
    static int GetId()
    {
        // the static here is part of declaration, \
        // which can be *ignored* on the subsequent calls of the function!!
        static auto id = nextId++;
        return id;
    }
    void ShowId()
    {
        std::cout << "selfId  " << GetId() << std::endl;
    }
};

int IComp::nextId = 0;
#endif
