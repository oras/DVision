#ifndef DEVICECAPTURESETTINGS_H
#define DEVICECAPTURESETTINGS_H

#include <QWidget>
#include <QObject>
#include <QApplication>
#include <QMutex>
#include <QWaitCondition>

namespace Ui {
class DeviceCaptureSettings;
}

class DeviceCaptureSettings : public QWidget
{
    Q_OBJECT

public:

     DeviceCaptureSettings(QWidget *parent=0);
    ~DeviceCaptureSettings();


private:
    Ui::DeviceCaptureSettings *ui;


};

#endif // DEVICECAPTURESETTINGS_H
