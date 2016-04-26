#ifndef IMGTOOLFACTORY_H
#define IMGTOOLFACTORY_H

#include <iostream>
#include <string>

using namespace std;

class ImgToolFactory
{
public:
    static ImgToolFactory *makeObject(string name);

    virtual void printInfo()=0;
};

class Square : public ImgToolFactory
{
public:
    void printInfo(){
        cout<<"Hello I am square"<<endl;
    }
};

class Circle : public ImgToolFactory
{
public:
    void printInfo(){
        cout<<"Hello I am circle"<<endl;
    }
};

class Horizon : public ImgToolFactory
{
public:
    void printInfo(){
        cout<<"Hello I am horinzon"<<endl;
    }
};

#endif // IMGTOOLFACTORY_H
