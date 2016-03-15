#include <QThread>
#include <QtGlobal>
#include <QtDebug>
#include "VStreamSimulator.h"
#include <UASManager.h>
#include <LinkManager.h>


VStreamSimulator* VStreamSimulator::instance(){
    static VStreamSimulator* _instance=0;

    if(_instance==0){
        _instance=new VStreamSimulator();

        // Set the application as parent to ensure that this object
        // will be destroyed when the main application exits
        _instance->setParent(qApp);
    }

    return _instance;
}
VStreamSimulator::VStreamSimulator(QObject *parent)
    :QThread(parent)
{
    go=true;
    con=false;
    link=NULL;

    connect(UASManager::instance(),SIGNAL(UASCreated(UASInterface*)),this,SLOT(UASCreated(UASInterface*)));
    connect(LinkManager::instance(),SIGNAL(messageReceived(LinkInterface*,mavlink_message_t)),this,SLOT(messageReceived(LinkInterface*,mavlink_message_t)));

    start(LowPriority);
}

void VStreamSimulator::streamVideo()
{
    int delay = (1000/frameRate);
    while(this->con){
        if (!capture.read(frame))
        {
            this->con = true;
        }
        if (frame.channels()== 3){
            cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
            img = QImage((const unsigned char*)(RGBframe.data),
                              RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
        }
        else
        {
            img = QImage((const unsigned char*)(frame.data),
                                 frame.cols,frame.rows,QImage::Format_Indexed8);
        }
        emit streamImage(img);
        this->msleep(delay);
    }

    emit videoStreamDisconnected();

}

bool VStreamSimulator::openVideoStream(String filename) {
    capture.open(filename);
    if (capture.isOpened())
    {
        frameRate = (int) capture.get(CV_CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

VStreamSimulator::~VStreamSimulator(){
    mutex.lock();
    con = false;
    go=false;
    capture.release();
    condition.wakeOne();
    mutex.unlock();
    if(link!=NULL){
        delete link;
        link=NULL;
    }
    wait();
}

void VStreamSimulator::heartbeatTimeout(bool timeout, unsigned int ms){
   this->con=false;
   link=NULL;

}

void VStreamSimulator::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}

void VStreamSimulator::run(){
     String filename="/home/or/Videos/Drone_fire_demo.mp4";

     while(go){
        if(link!=NULL){
            connect(uasMain, SIGNAL(heartbeatTimeout(bool, unsigned int)), this, SLOT(heartbeatTimeout(bool, unsigned int)));
            connect(UASManager::instance(),SIGNAL(UASCreated(UASInterface*)),this,SLOT(UASCreated(UASInterface*)));
            this->con=true;
            openVideoStream(filename);
            streamVideo();
        }

        sleep(1);
    }
}

void VStreamSimulator::UASCreated(UASInterface *UAS){
    QList<LinkInterface*> *linkList;

    // Get a list of all existing UAS
    foreach (UASInterface* uas, UASManager::instance()->getUASList()) {
         linkList=uas->getLinks();

         for(int i=0;linkList->at(i)!=NULL;i++){
            if(linkList->at(i)->isConnected()){
                link=linkList->at(i);
                uasMain=uas;
                break;
            }
         }

         if(link!=NULL)
             break;
    }
}

void VStreamSimulator::messageReceived(LinkInterface *link, mavlink_message_t message){
     QList<LinkInterface*> *linkList=uasMain->getLinks();

    for(int i=0;linkList->at(i)!=NULL;i++){
       if(linkList->at(i)->isConnected()){
           this->link=linkList->at(i);
           break;
       }
    }
}
