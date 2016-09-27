#include "EventsDetection.h"
#include <qmessagebox.h>
#include <QThread>
#include <QtGlobal>
#include <QtDebug>
#include <QApplication>
#include <VStreamSimulator.h>
#include <CameraCapture.h>
#include <QSettings>
#include <QGCCore.h>
#include "DeviceCaptureSettings.h"
#include <MainWindow.h>
#include "IRCalibration.h"


EventsDetection* EventsDetection::instance(QObject *parent){
    static EventsDetection* _instance=0;

    if(_instance==0){
        _instance=new EventsDetection(parent);

        // Set the application as parent to ensure that this object
        // will be destroyed when the main application exits
        //_instance->setParent(qApp);
    }

    return _instance;
}

EventsDetection::EventsDetection(QObject *parent)
    :QThread(parent)
{


        connect(VStreamSimulator::instance(),SIGNAL(streamImage(const QImage&)), this, SLOT(streamImage(const QImage&)));
        connect(VStreamSimulator::instance(),SIGNAL(videoStreamDisconnected()), this, SLOT(videoStreamDisconnected()));
        connect(CameraCapture::instance(),SIGNAL(streamImage(const QImage&)), this, SLOT(streamImage(const QImage&)));
        connect(CameraCapture::instance(),SIGNAL(videoStreamDisconnected()), this, SLOT(videoStreamDisconnected()));
        connect(dynamic_cast<MainWindow*>(parent)->irc,SIGNAL(updateImageIRDiff(const cv::Point p)),this,SLOT(updateImageIRDiff(const cv::Point p)));

     imgReady=false;
     warn=new WarnSound();
     m_pauseRequired=true;

     soundWarn=true;
     go=true;

     lastTime=clock();
     vCircle=NULL;

     loadDiskDefaultImg();

     start(LowPriority);
}

EventsDetection::~EventsDetection(){
    mutex.lock();
    go=false;
    m_pauseRequired=false;
    m_pauseManager.wakeAll();
    mutex.unlock();

    wait();
}

void EventsDetection::registerNewEventToDetect(string name){
    Event *newEvent=Event::newEvent(name);

    newEvent->setName(name);

    // Check if exist and update the pointer to the new settings of the existing
    // event kind.

    Event *p;

    for(p=this->head;p!=NULL;p=p->getNext()){
        if(p->getEventName()==name){
            delete p;
            p=newEvent;
            return;
        }
    }

    //If no exist add it to this list:
    if(this->head==NULL){
        this->head=newEvent;

    }
    else{
        Event *curr= newEvent;
        curr->setNext(this->head);
        this->head=curr;
    }

}

void EventsDetection::removeEvent(Event *event){
    if(this->head!=NULL){
        // Remove head:
        if(event->getEventName()==this->head->getEventName()){
            Event *curr=this->head->getNext();
            delete this->head;
            this->head=curr;
        }

        // Remove between:
        Event *p;
        for(p=this->head->getNext();p!=NULL;p=p->getNext()){
            if(p->getEventName()==event->getEventName()){
                if(p->getNext()!=NULL){
                    Event *curr=p->getNext();
                    delete p;
                    p=curr;
                }
                else{
                     //Remove tail:
                     delete p;
                     p=NULL;
                }

            }
        }

    }
}

void EventsDetection::startDetection(){
    QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);


    for(int i=0;i<4;i++){
         int ret =msgBox.exec();


    }
}

void EventsDetection::run(){
    cout<<"Events Detection has been loaded!"<<endl;

    while(go){

        // Hold detection if required.
        if(m_pauseRequired)
            m_pauseManager.wait(&m_continue);

        // Resume
        if(imgReady){

            thisTime=clock();

            if((thisTime-lastTime>3*NUM_SECONDS)){

                //while(irt::m_continue.tryLock());
                     matImg=ImgRecoTool::QImageToCvMat(img);
                     matImgIR=ImgRecoTool::QImageToCvMat(imgIR);
                //irt::m_continue.unlock();

                this->root=ImgRecoTool::getSaliencyMap(matImg,1,100);
                //ImgRecoTool::contrast(matImg,100);
                this->hRoot=ImgRecoTool::createHorizon(50,matImg,170);

                Horizon* horizon=new Horizon(this->hRoot,50);

                if(vCircle!=NULL)
                    vCircle->clear();

                vCircle=ImgRecoTool::markCircleOnFlame(matImg,matImgIR,this->root,horizon,diffX,diffY);

                if(!vCircle->empty()){
                    if(soundWarn){
                        warn->playWarnSound(LOCK);
                        soundWarn=false;
                    }

                    // Check if new event deteted, capsulate it and report to log & Google map.

                }
                else {
                    warn->playWarnSound(STOP_LOCK);
                    soundWarn=true;

                }


                ImgRecoTool::releaseHRoot(hRoot);
                ImgRecoTool::freeLinkedList<irt::Node>(root);

                delete horizon;

                lastTime=thisTime;

                emit imgInfoReady();

            }

            imgReady=false;
        }
    }
}

void EventsDetection::streamImage(const QImage &image){
    if(!imgReady){
        resume();
        while(irt::m_continue.tryLock());
         this->img=image.copy();
        irt::m_continue.unlock();
        imgReady=true;
    }
}

void EventsDetection::streamImageIR(const QImage &image){
    if(!imgIRReady){
        resumeIR();
        while(irt::mIR_continue.tryLock());
            imgIR=image.copy();
        irt::mIR_continue.unlock();
        imgIRReady=true;
    }
}

void EventsDetection::resume(){
    m_pauseRequired=false;
    m_pauseManager.wakeAll();
}

void EventsDetection::resumeIR(){
    mIR_pauseRequired=false;
    mIR_pauseManager.wakeAll();
}

void EventsDetection::pause(){
    m_pauseRequired=true;
}

void EventsDetection::pauseIR(){
    mIR_pauseRequired=true;
}

bool EventsDetection::isPause(){
    return m_pauseRequired;
}

bool EventsDetection::isPauseIR(){
    return mIR_pauseRequired;
}

void EventsDetection::videoStreamDisconnected(){
    pause();
}

std::vector<Circ> EventsDetection::getCircleVector(){
    return *vCircle;
}

void EventsDetection::updateImageIRDiff(const cv::Point p){
    this->diffX=p.x;
    this->diffY=p.y;
}

void EventsDetection::loadDiskDefaultImg(){
    QFile mFile("/home/or/workspace/apm_planner/CalibrationData.txt");

    if(mFile.open(QIODevice::ReadOnly)){
        QTextStream in(&mFile);

        this->diffX=in.readLine().toInt();
        this->diffY=in.readLine().toInt();


        mFile.close();
    }
}

