#include "HUD.h"
#include "ui_HUD.h"

HUD::HUD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HUD)
{
    ui->setupUi(this);
}

HUD::~HUD()
{
    delete ui;
}
