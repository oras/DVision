#include "Horizon.h"

Horizon::Horizon(cv::Point* arr, const int &size){
    this->arr=arr;
    this->size=size;
}


cv::Point* Horizon::getArr(){
    return this->arr;
}

int Horizon::getSize(){
    return this->size;
}

cv::Point* Horizon::getForwardHorizonPoint(const int &x, const int &imgColSize){
    double unit=(double)size/imgColSize;
    int index=(int)x*unit+1;
    return &arr[index];
}

cv::Point* Horizon::getPrevHorizonPoint(const int &x, const int &imgColSize){
    double unit=(double)size/imgColSize;
    int index=(int)x*unit;
    return &arr[index];
}

