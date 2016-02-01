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

#ifndef POSITION_H
#define POSITION_H


class Position{
private:
    float N;
    float E;
    long altitude;
public:
    Position(float n, float e, long altitude);

    float* getPosition();

    long getAltitude();

    float getNorth();

    float getEast();
};

#endif // POSITION_H
