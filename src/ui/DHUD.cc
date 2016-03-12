#include "DHUD.h"
#include "ui_DHUD.h"
#include "configuration.h"
#include <mavlink.h>
#include <UASManager.h>
#include <LinkManager.h>


DHUD::DHUD(QPointer<MAVLinkSimulationLink> mavlink,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DHUD)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("qproperty-alignment: 'AlignCenter';font-size: 40px;");
    QList<LinkInterface*> *linkList;


    // Get a list of all existing UAS
    UASInterface* uasMain;

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


    connect(link,SIGNAL(connected()),this,SLOT(connectedLink()));
    connect(uasMain, SIGNAL(heartbeatTimeout(bool, unsigned int)), this, SLOT(heartbeatTimeout(bool, unsigned int)));





}

void DHUD::connectedLink(){
    ui->label->setText("Connected!");
}

void DHUD::heartbeatTimeout(bool timeout, unsigned int ms){
    ui->label->setText("No Signal!");
}

void DHUD::showCriticalMessage(const QString& title, const QString& message){
//    QMessageBox msgBox(this);
    //QMessageBox::information(this,title,message);
    ui->label->setText(title+message);
//    msgBox.setIcon(QMessageBox::Critical);
//    msgBox.setText(title);
//    msgBox.setInformativeText(message);
//    msgBox.setStandardButtons(QMessageBox::Ok);
//    msgBox.setDefaultButton(QMessageBox::Ok);
//    msgBox.show();
}

void DHUD::establishMAV(){



}

DHUD::~DHUD(){
    delete ui;
}
