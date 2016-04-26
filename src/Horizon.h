/*=============================================================================
 * EVENTS DITECTION USING THE GROUNDCONTROL PROJECT
 *
 * This code is part of the JCE Software & Computer Enginnering Department
 * research project, Written by Asnin Or All rights 2015-2016.
 *
 * www.jce.ac.il
 *
 *
===============================================================================*/

#ifndef HORIZON_H
#define HORIZON_H

#include <opencv/cv.h>

using namespace cv;

class Horizon
{
private:
    cv::Point *arr;
    int size;
public:
    Horizon(cv::Point* arr,const int &size);

    int getSize();

    cv::Point* getArr();

    cv::Point* getPrevHorizonPoint(const int &x,const int &imgColSize);

    cv::Point* getForwardHorizonPoint(const int &x, const int &imgColSize);
};

#endif // HORIZON_H
