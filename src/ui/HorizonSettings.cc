#include "HorizonSettings.h"
#include "ui_HorizonSettings.h"

HorizonSettings::HorizonSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HorizonSettings)
{
    ui->setupUi(this);
}

HorizonSettings::~HorizonSettings()
{
    delete ui;
}
