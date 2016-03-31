/*=============================================================================
 * EVENTS DITECTION USING THE GROUNDCONTROL PROJECT
 *
 * This code is part of the JCE Software & Computer Enginnering Department
 * research project, Written by Asnin Or All rights 2015-2016.
 *
 * www.jce.ac.il
 *
 * This is an event detection loop.
 * all kinds of event to detecd are loaded include there seetings.
===============================================================================*/

#ifndef EVENTSDETECTION_H
#define EVENTSDETECTION_H

#include <Event.h>
#include <Horizon.h>
#include <string>
#include <EventFactory.h>
#include <QThread>
#include <QObject>

class EventsDetection : public QThread{
    Q_OBJECT
private:
    Event *head;
    Horizon *horizon;
public:
    static EventsDetection* instance();
    ~EventsDetection();

    void registerNewEventToDetect(string name);

    void startDetection();
public slots:
signals:
private:
    //void logEvent(Event *event);

    //int* calculatePrecentOfFireDetect();

    //void markEventOnGoogleEarth(Event *event);

    void removeEvent(Event *event);

    //void readUAVinfo();

    void start();

    //void stop();

    EventsDetection(QObject *parent=0);
protected:
    void run();
};

#endif // EVENTSDETECTION_H
