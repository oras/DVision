#include "firereco.h"
#include "ui_firereco.h"

FireReco::FireReco(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FireReco)
{
    ui->setupUi(this);
}

FireReco::~FireReco()
{
    delete ui;
}
