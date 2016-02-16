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

#include "Event.h"

Event *Event::newEvent(const string &description){
    if(description=="Flame")
        return new Flame();
    else if(description=="Smoke")
        return new Smoke();
    else if(description=="Heat")
        return new Heat();

    return NULL;
}


void Event::setImage(string path){
    this->img=path;
}

void Event::setVideo(string path){
    this->video=path;
}

void Event::setPosition(Position &position){
    //Need to add a check mode for cordinate position format.

    this->pos=new Position(position.getNorth(),position.getEast(),
                           position.getAltitude());
}

void Event::setName(string name){
    this->eventName=name;
}

string Event::getImage(){
    return this->img;
}

string Event::getEventName(){
    return this->eventName;
}

int Event::getBinaryImgThreshold(){
    return this->binaryImgThreshold;
}

int Event::getSensitivity(){
    return this->sensitivity;
}

bool Event::getScanMethode(){
    return this->scanMethode;
}

int Event::getDetectedAreaSize(){
    return this->detectedAreaSize;
}

int Event::getMarkAreaDefinitionResolution(){
    return this->markAreaDefinitionResolution;
}

void Event::setNext(Event *next){
    this->next=next;
}

Event *Event::getNext(){
    return this->next;
}

Event::~Event(){

}
