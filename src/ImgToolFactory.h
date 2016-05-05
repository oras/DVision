#ifndef IMGTOOLFACTORY_H
#define IMGTOOLFACTORY_H

#include <iostream>
#include <string>
#include <opencv/cv.h>

using namespace std;

class ImgToolFactory
{

public:
     cv::Point *point;

    static ImgToolFactory *makeObject(int name,cv::Point point[]);
};

class Squa: public ImgToolFactory
{
private:
    int rad;
public:
    Squa(cv::Point* point){
        this->point=point;
    }
};

class Circ : public ImgToolFactory
{
private:
    int rad;
    Circ* next;
public:
    Circ(cv::Point* point){
        this->point=point;

        rad=(point[1].y>point[1].x?point[1].y:point[1].x);

        if(rad<40)
            rad=40;
    }

    cv::Point getCircleCentralPoint(){
        return point[0];
    }

    int getCircleRadios(){
        return rad;
    }

    void setNext(Circ* next){
        this->next=next;
    }

    Circ* getNext(){
        return next;
    }
};


#endif // IMGTOOLFACTORY_H
