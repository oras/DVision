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
 * If any detection return true, sound of alarm will play include Warning
 * message. All detected events will be logged into a log file & display on
 * the Goole earth navigation map.
===============================================================================*/

#ifndef EVENTSDETECTION_H
#define EVENTSDETECTION_H

#include <Event.h>
#include <Horizon.h>
#include <string>
#include <EventFactory.h>
#include <QThread>
#include <QObject>
#include <ImgRecoTool.h>
#include <WarnSound.h>
#include <QMutex>
#include <QWaitCondition>

using namespace irt;
using namespace cv;

class EventsDetection : public QThread{
    Q_OBJECT
private:
    Event* head;
    Horizon* horizon;
    QImage img;
    bool imgReady, go;
    bool soundWarn;
    cv::Point *hRoot;
    Node* root;
    WarnSound* warn;
    bool m_pauseRequired;
    QMutex m_continue;
    QMutex mutex;
    QWaitCondition m_pauseManager;

public:
    static EventsDetection* instance();
    ~EventsDetection();

    void registerNewEventToDetect(string name);

    void pause();

    bool isPause();

    void resume();

    void startDetection();
public slots:
    /** @brief Update image from video stream */
    void streamImage(const QImage &image);
    /** @brief Video stream has been disconnected */
    void videoStreamDisconnected();
signals:
    void imageReady(const QImage &img);
private:
    //void logEvent(Event *event);

    //int* calculatePrecentOfFireDetect();

    //void markEventOnGoogleEarth(Event *event);

     void run();

    void removeEvent(Event *event);

    //void readUAVinfo();

    //void stop();

    EventsDetection(QObject *parent=0);
protected:
};

#endif // EVENTSDETECTION_H
