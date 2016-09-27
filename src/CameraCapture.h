#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <QObject>
#include <ImgRecoTool.h>
#include <QThread>
 #include <QApplication>

using namespace cv;
using namespace irt;

class CameraCapture: public QThread
{
    Q_OBJECT
public:
    static CameraCapture* instance();
private:
    bool con,go,IROnline;
    Mat frameIR, frame;
    int cameraNum;
    double dWidth, dheight;
    double dWidthIR, dheightIR;
    int frameRateIR;
    int frameRate;
    Mat RGBframe,RGBframeIR;
    QImage img,imgIR;
    VideoCapture *cap;
    VideoCapture *capIR;
    QMutex mutex;


    CameraCapture(QObject *parent=0);
    ~CameraCapture();

    void initalizeCameraMounted();

    void streamVideo();

    void streamIR();
signals:
    // Signal to output frame to be displayed
    void streamImage(const QImage &image);
    // Signal to output frame IR to be displayed
    void streamImageIR(const QImage &image);
    // Signal video stream has been disconnected
    void videoStreamDisconnected();
protected:
     void run();
};

#endif // CAMERACAPTURE_H
