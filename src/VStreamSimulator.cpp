#include <QThread>
#include <QtGlobal>
#include <QtDebug>
#include "VStreamSimulator.h"


VStreamSimulator* VStreamSimulator::instance(){
    static VStreamSimulator* _instance=0;

    if(_instance==0){
        _instance=new VStreamSimulator();

        // Set the application as parent to ensure that this object
        // will be destroyed when the main application exits
        _instance->setParent(qApp);
    }

    return _instance;
}
VStreamSimulator::VStreamSimulator(QObject *parent)
    :QThread(parent)
{
    this->connect=true;
}

void VStreamSimulator::streamVideo()
{
    int delay = (1000/frameRate);
    while(this->connect){
        if (!capture.read(frame))
        {
            this->connect = true;
        }
        if (frame.channels()== 3){
            cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
            img = QImage((const unsigned char*)(RGBframe.data),
                              RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
        }
        else
        {
            img = QImage((const unsigned char*)(frame.data),
                                 frame.cols,frame.rows,QImage::Format_Indexed8);
        }
        emit processedImage(img);
        this->msleep(delay);
    }
}

bool VStreamSimulator::openVideoStream(String filename) {
    capture.open(filename);
    if (capture.isOpened())
    {
        frameRate = (int) capture.get(CV_CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

VStreamSimulator::~VStreamSimulator(){
    mutex.lock();
    connect = false;
    capture.release();
    condition.wakeOne();
    mutex.unlock();
    wait();
}

void VStreamSimulator::heartbeatTimeout(bool timeout, unsigned int ms){
    this->connect=false;
}

void VStreamSimulator::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}

void VStreamSimulator::connected(){
    this->connect=true;
}
