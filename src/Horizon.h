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


class Horizon
{
private:
    int *point;
    Horizon *next;
public:
    Horizon(int x, int y);

    int* getPoint();

    void setNext(Horizon *next);

    Horizon* getNext();
};

#endif // HORIZON_H
