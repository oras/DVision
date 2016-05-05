#include "ImgToolFactory.h"
#include <utility>

ImgToolFactory* ImgToolFactory::makeObject(int name,cv::Point *point){
    switch(name){

    case 0:
    {
        return new Squa(point);
        break;
    }
    case 1:
    {
        return new Circ(point);
        break;
    }
    default:
        return NULL;
    }
}


