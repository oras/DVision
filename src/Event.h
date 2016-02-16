/*=============================================================================
 * EVENTS DITECTION USING THE GROUNDCONTROL PROJECT
 *
 * This code is part of the JCE Software & Computer Enginnering Department
 * research project, Written by Asnin Or All rights 2015-2016.
 *
 * www.jce.ac.il
 *
 * This is an event object include all event data. position of event Altitude
 * path to screen shot, short video. Algorithem recognition settings etc.
===============================================================================*/

#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <Position.h>
#include <stdio.h>
#include <iostream>

using namespace std;
class Event
{
private:
    string eventName;
    int binaryImgThreshold;
    int sensitivity;
    float resolution;
    bool scanMethode;
    Position *pos;
    string img;
    string video;
    int detectedAreaSize;
    int markAreaDefinitionResolution;
    Event *next;
public:
    static Event *newEvent(const string &description);

    void setImage(string path);

    void setVideo(string path);

    void setPosition(Position &position);

    void setName(string name);

    string getImage();

    string getEventName();

    int getBinaryImgThreshold();

    int getSensitivity();

    bool getScanMethode();

    int getDetectedAreaSize();

    int getMarkAreaDefinitionResolution();

    void setNext(Event *event);

    Event *getNext();

    virtual void detect()=0;

    ~Event();

};

class Flame : public Event{
public:

    void detect(){
        cout<<"Hellow I am a flame";
    }
};

class Smoke : public Event{
public:


    void detect(){
        cout<<"Hellow I am a smoke";
    }
};

class Heat : public Event{
public:


    void detect(){
        cout<<"Hellow I am a heat";
    }
};

#endif // EVENT_H
