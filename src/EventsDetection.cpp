#include "EventsDetection.h"
#include <qmessagebox.h>
#include <QThread>
#include <QtGlobal>
#include <QtDebug>
#include <QApplication>
#include <VStreamSimulator.h>

EventsDetection* EventsDetection::instance(){
    static EventsDetection* _instance=0;

    if(_instance==0){
        _instance=new EventsDetection();

        // Set the application as parent to ensure that this object
        // will be destroyed when the main application exits
        _instance->setParent(qApp);
    }

    return _instance;
}

EventsDetection::EventsDetection(QObject *parent)
    :QThread(parent)
{
     connect(VStreamSimulator::instance(),SIGNAL(streamImage(const QImage&)), this, SLOT(streamImage(const QImage&)));
     connect(VStreamSimulator::instance(),SIGNAL(videoStreamDisconnected()), this, SLOT(videoStreamDisconnected()));

     imgReady=false;
     warn=new WarnSound();
     m_pauseRequired=true;

     soundWarn=true;
     go=true;

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
    Mat matImg;

    while(go){

        // Hold detection if required.
        if(m_pauseRequired)
            m_pauseManager.wait(&m_continue);

        // Resume
        if(imgReady){

            matImg=ImgRecoTool::QImageToCvMat(img);
            this->root=ImgRecoTool::getSaliencyMap(matImg,1,100);
            //ImgRecoTool::contrast(matImg,100);
            this->hRoot=ImgRecoTool::createHorizon(50,matImg,170);

            Horizon* horizon=new Horizon(this->hRoot,50);

            if(ImgRecoTool::markCircleOnFlame(matImg,this->root,horizon)){
                if(soundWarn){
                    warn->playWarnSound(irt::LOCK);
                    soundWarn=false;
                }

                // Check if new event deteted, capsulate it and report to log & Google map.

            }
            else {
                warn->playWarnSound(irt::STOP_LOCK);
                soundWarn=true;

            }

            img=ImgRecoTool::cvMatToQImage(matImg);

            ImgRecoTool::releaseHRoot(hRoot);
            ImgRecoTool::freeLinkedList<Node>(root);

            delete horizon;

            emit imageReady(img);

            imgReady=false;
        }
    }
}

void EventsDetection::streamImage(const QImage &image){
    if(!imgReady){
        resume();
        this->img=image;
        imgReady=true;
    }
}

void EventsDetection::resume(){
    m_pauseRequired=false;
    m_pauseManager.wakeAll();
}

void EventsDetection::pause(){
    m_pauseRequired=true;
}

bool EventsDetection::isPause(){
    return m_pauseRequired;
}

void EventsDetection::videoStreamDisconnected(){
    pause();
}

