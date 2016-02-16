#include "FireHeatReco.h"
#include "ui_FireHeatReco.h"

FireHeatReco::FireHeatReco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FireHeatReco)
{
    ui->setupUi(this);
}

FireHeatReco::~FireHeatReco()
{
    delete ui;
}
