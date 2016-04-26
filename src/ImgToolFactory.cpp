#include "ImgToolFactory.h"
#include <utility>

ImgToolFactory* ImgToolFactory::makeObject(string name){
    name=std::tolower(name,std::locale());
    if(name.compare("square"))
        return new Square;
    else if(name.compare("circle"))
        return new Circle;
    else if(name.compare("horizon"))
        return new Horizon;
    else
        return NULL;
}
