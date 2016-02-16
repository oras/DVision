#include "Position.h"

Position::Position(float n,float e, long altitude){
    this->N=n;
    this->E=e;
    this->altitude=altitude;
}

float* Position::getPosition(){
    static float arr[]={this->N,this->E};

    return arr;
}

long Position::getAltitude(){
    return this->altitude;
}

float Position::getNorth(){
    return this->N;
}

float Position::getEast(){
    return this->E;
}
