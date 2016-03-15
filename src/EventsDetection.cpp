#include "EventsDetection.h"
#include <qmessagebox.h>
#include <QThread>
#include <QtGlobal>
#include <QtDebug>
#include <QApplication>

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
    start(LowPriority);
}

EventsDetection::~EventsDetection(){
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

void EventsDetection::start(){
   // QtConcurrent::run(this,&EventsDetection::startDetection);
}

void EventsDetection::run(){

}
