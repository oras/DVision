#include "DeviceCaptureSettings.h"
#include "ui_DeviceCaptureSettings.h"
#include <unistd.h>

DeviceCaptureSettings::DeviceCaptureSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceCaptureSettings)
{
    ui->setupUi(this);

}



DeviceCaptureSettings::~DeviceCaptureSettings()
{
    delete ui;
}
