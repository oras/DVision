#include "DHUD.h"
#include "ui_DHUD.h"
#include "configuration.h"
#include "mavlink.h"

DHUD::DHUD(QPointer<MAVLinkSimulationLink> mavlink,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DHUD)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("qproperty-alignment: 'AlignCenter';font-size: 40px;");

    mavlink->messageReceived();
}

void DHUD::establishMAV(){



}

DHUD::~DHUD(){
    delete ui;
}
